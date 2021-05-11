/******************************************************************************\
 * Copyright (c) 2004-2014
 *
 * Author(s):
 *  Volker Fischer
 *
 ******************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
\******************************************************************************/

#include "util.h"


/* Implementation *************************************************************/
// Input level meter implementation --------------------------------------------
void CStereoSignalLevelMeter::Update ( const CVector<short>& vecsAudio )
{
    // get the stereo vector size
    const int iStereoVecSize = vecsAudio.Size();

    // Get maximum of current block
    //
    // Speed optimization:
    // - we only make use of the positive values and ignore the negative ones
    //   -> we do not need to call the fabs() function
    // - we only evaluate every third sample
    //
    // With these speed optimizations we might loose some information in
    // special cases but for the average music signals the following code
    // should give good results.
    //
    short sMaxL = 0;
    short sMaxR = 0;

    for ( int i = 0; i < iStereoVecSize; i += 6 ) // 2 * 3 = 6 -> stereo
    {
        // left channel
        sMaxL = std::max ( sMaxL, vecsAudio[i] );

        // right channel
        sMaxR = std::max ( sMaxR, vecsAudio[i + 1] );
    }

    dCurLevelL = UpdateCurLevel ( dCurLevelL, sMaxL );
    dCurLevelR = UpdateCurLevel ( dCurLevelR, sMaxR );
}

double CStereoSignalLevelMeter::UpdateCurLevel ( double       dCurLevel,
                                                 const short& sMax )
{
    // decrease max with time
    if ( dCurLevel >= METER_FLY_BACK )
    {
// TODO calculate factor from sample rate
        dCurLevel *= 0.95;
    }
    else
    {
        dCurLevel = 0;
    }

    // update current level -> only use maximum
    if ( static_cast<double> ( sMax ) > dCurLevel )
    {
        return static_cast<double> ( sMax );
    }
    else
    {
        return dCurLevel;
    }
}

double CStereoSignalLevelMeter::CalcLogResult ( const double& dLinearLevel )
{
    const double dNormMicLevel = dLinearLevel / _MAXSHORT;

    // logarithmic measure
    if ( dNormMicLevel > 0 )
    {
        return 20.0 * log10 ( dNormMicLevel );
    }
    else
    {
        return -100000.0; // large negative value
    }
}


// CRC -------------------------------------------------------------------------
void CCRC::Reset()
{
    // init state shift-register with ones. Set all registers to "1" with
    // bit-wise not operation
    iStateShiftReg = ~uint32_t ( 0 );
}

void CCRC::AddByte ( const uint8_t byNewInput )
{
    for ( int i = 0; i < 8; i++ )
    {
        // shift bits in shift-register for transistion
        iStateShiftReg <<= 1;

        // take bit, which was shifted out of the register-size and place it
        // at the beginning (LSB)
        // (If condition is not satisfied, implicitely a "0" is added)
        if ( ( iStateShiftReg & iBitOutMask) > 0 )
        {
            iStateShiftReg |= 1;
        }

        // add new data bit to the LSB
        if ( ( byNewInput & ( 1 << ( 8 - i - 1 ) ) ) > 0 )
        {
            iStateShiftReg ^= 1;
        }

        // add mask to shift-register if first bit is true
        if ( iStateShiftReg & 1 )
        {
            iStateShiftReg ^= iPoly;
        }
    }
}

uint32_t CCRC::GetCRC()
{
    // return inverted shift-register (1's complement)
    iStateShiftReg = ~iStateShiftReg;

    // remove bit which where shifted out of the shift-register frame
    return iStateShiftReg & ( iBitOutMask - 1 );
}


/******************************************************************************\
* Audio Reverberation                                                          *
\******************************************************************************/
/*
    The following code is based on "JCRev: John Chowning's reverberator class"
    by Perry R. Cook and Gary P. Scavone, 1995 - 2004
    which is in "The Synthesis ToolKit in C++ (STK)"
    http://ccrma.stanford.edu/software/stk

    Original description:
    This class is derived from the CLM JCRev function, which is based on the use
    of networks of simple allpass and comb delay filters. This class implements
    three series allpass units, followed by four parallel comb filters, and two
    decorrelation delay lines in parallel at the output.
*/
void CAudioReverb::Init ( const int    iSampleRate,
                          const double rT60 )
{
    int delay, i;

    // delay lengths for 44100 Hz sample rate
    int lengths[9] = { 1116, 1356, 1422, 1617, 225, 341, 441, 211, 179 };
    const double scaler = (double) iSampleRate / 44100.0;

    if ( scaler != 1.0 )
    {
        for ( i = 0; i < 9; i++ )
        {
            delay = (int) floor ( scaler * lengths[i] );

            if ( ( delay & 1 ) == 0 )
            {
                delay++;
            }

            while ( !isPrime ( delay ) )
            {
                delay += 2;
            }

            lengths[i] = delay;
        }
    }

    for ( i = 0; i < 3; i++ )
    {
        allpassDelays[i].Init ( lengths[i + 4] );
    }

    for ( i = 0; i < 4; i++ )
    {
        combDelays[i].Init ( lengths[i] );
        combFilters[i].setPole ( 0.2 );
    }

    setT60 ( rT60, iSampleRate );
    outLeftDelay.Init ( lengths[7] );
    outRightDelay.Init ( lengths[8] );
    allpassCoefficient = (double) 0.7;
    Clear();
}

bool CAudioReverb::isPrime ( const int number )
{
/*
    Returns true if argument value is prime. Taken from "class Effect" in
    "STK abstract effects parent class".
*/
    if ( number == 2 )
    {
        return true;
    }

    if ( number & 1 )
    {
        for ( int i = 3; i < (int) sqrt ( (double) number ) + 1; i += 2 )
        {
            if ( ( number % i ) == 0 )
            {
                return false;
            }
        }

        return true; // prime
    }
    else
    {
        return false; // even
    }
}

void CAudioReverb::Clear()
{
    // reset and clear all internal state
    allpassDelays[0].Reset ( 0 );
    allpassDelays[1].Reset ( 0 );
    allpassDelays[2].Reset ( 0 );
    combDelays[0].Reset ( 0 );
    combDelays[1].Reset ( 0 );
    combDelays[2].Reset ( 0 );
    combDelays[3].Reset ( 0 );
    combFilters[0].Reset();
    combFilters[1].Reset();
    combFilters[2].Reset();
    combFilters[3].Reset();
    outRightDelay.Reset ( 0 );
    outLeftDelay.Reset ( 0 );
}

void CAudioReverb::setT60 ( const double rT60,
                            const int    iSampleRate )
{
    // set the reverberation T60 decay time
    for ( int i = 0; i < 4; i++ )
    {
        combCoefficient[i] = pow ( (double) 10.0, (double) ( -3.0 *
            combDelays[i].Size() / ( rT60 * iSampleRate ) ) );
    }
}

void CAudioReverb::COnePole::setPole ( const double dPole )
{
    // calculate IIR filter coefficients based on the pole value
    dA = -dPole;
    dB = 1.0 - dPole;
}

double CAudioReverb::COnePole::Calc ( const double dIn )
{
    // calculate IIR filter
    dLastSample = dB * dIn - dA * dLastSample;

    return dLastSample;
}

void CAudioReverb::ProcessSample ( int16_t&     iInputOutputLeft,
                                   int16_t&     iInputOutputRight,
                                   const double dAttenuation )
{
    // compute one output sample
    double temp, temp0, temp1, temp2;

    // we sum up the stereo input channels (in case mono input is used, a zero
    // shall be input for the right channel)
    const double dMixedInput = 0.5 * ( iInputOutputLeft + iInputOutputRight );

    temp = allpassDelays[0].Get();
    temp0 = allpassCoefficient * temp;
    temp0 += dMixedInput;
    allpassDelays[0].Add ( temp0 );
    temp0 = - ( allpassCoefficient * temp0 ) + temp;

    temp = allpassDelays[1].Get();
    temp1 = allpassCoefficient * temp;
    temp1 += temp0;
    allpassDelays[1].Add ( temp1 );
    temp1 = - ( allpassCoefficient * temp1 ) + temp;

    temp = allpassDelays[2].Get();
    temp2 = allpassCoefficient * temp;
    temp2 += temp1;
    allpassDelays[2].Add ( temp2 );
    temp2 = - ( allpassCoefficient * temp2 ) + temp;

    const double temp3 = temp2 + combFilters[0].Calc ( combCoefficient[0] * combDelays[0].Get() );
    const double temp4 = temp2 + combFilters[1].Calc ( combCoefficient[1] * combDelays[1].Get() );
    const double temp5 = temp2 + combFilters[2].Calc ( combCoefficient[2] * combDelays[2].Get() );
    const double temp6 = temp2 + combFilters[3].Calc ( combCoefficient[3] * combDelays[3].Get() );

    combDelays[0].Add ( temp3 );
    combDelays[1].Add ( temp4 );
    combDelays[2].Add ( temp5 );
    combDelays[3].Add ( temp6 );

    const double filtout = temp3 + temp4 + temp5 + temp6;

    outLeftDelay.Add ( filtout );
    outRightDelay.Add ( filtout );

    // inplace apply the attenuated reverb signal
    iInputOutputLeft  = Double2Short (
        ( 1.0 - dAttenuation ) * iInputOutputLeft +
        0.5 * dAttenuation * outLeftDelay.Get() );

    iInputOutputRight = Double2Short (
        ( 1.0 - dAttenuation ) * iInputOutputRight +
        0.5 * dAttenuation * outRightDelay.Get() );
}


/******************************************************************************\
* GUI Utilities                                                                *
\******************************************************************************/
// About dialog ----------------------------------------------------------------
CAboutDlg::CAboutDlg ( QWidget* parent ) : QDialog ( parent )
{
    setupUi ( this );

    // set the text for the about dialog html text control
    txvCredits->setOpenExternalLinks ( true );
    txvCredits->setText (
        "<p>" // general description of software
        "<big>" + tr( "The " ) + APP_NAME +
        tr ( " software enables musicians to perform real-time jam sessions "
        "over the internet. There is a " ) + APP_NAME + tr ( " "
        "server which collects the audio data from each " ) +
        APP_NAME + tr ( " client, mixes the audio data and sends the mix back "
        "to each client.") + "</big></p><br>"
        "<p><font face=\"courier\">" // GPL header text
        "This program is free software; you can redistribute it and/or modify "
        "it under the terms of the GNU General Public License as published by "
        "the Free Software Foundation; either version 2 of the License, or "
        "(at your option) any later version.<br>This program is distributed in "
        "the hope that it will be useful, but WITHOUT ANY WARRANTY; without "
        "even the implied warranty of MERCHANTABILITY or FITNESS FOR A "
        "PARTICULAR PURPOSE. See the GNU General Public License for more "
        "details.<br>You should have received a copy of the GNU General Public "
        "License along with his program; if not, write to the Free Software "
        "Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 "
        "USA"
        "</font></p><br>"
        "<p><b>" + APP_NAME + // libraries used by this compilation
        tr ( " uses the following libraries, resources or code snippets:" ) +
        "</b></p>"
        "<ul>"
        "<li>Qt cross-platform application framework: "
        "<i><a href=""http://qt.digia.com"">http://qt.digia.com</a></i></li>"
        "<li>Opus Interactive Audio Codec: "
        "<i><a href=""http://www.opus-codec.org"">http://www.opus-codec.org</a></i></li>"
        "<li>Audio reverberation code: by Perry R. Cook and Gary P. Scavone, "
        "1995 - 2004 (taken from "
        "<i><a href=""http://ccrma.stanford.edu/software/stk"">"
        "The Synthesis ToolKit in C++ (STK)</a></i>)</li>"
        "<li>Some pixmaps are from the Open Clip Art Library (OCAL): "
        "<i><a href=""http://openclipart.org"">http://openclipart.org</a></i></li>"
        "</ul>"
        "</center><br>");

    // set version number in about dialog
    lblVersion->setText ( GetVersionAndNameStr() );

    // set window title
    setWindowTitle ( tr ( "About " ) + APP_NAME );
}

QString CAboutDlg::GetVersionAndNameStr ( const bool bWithHtml )
{
    QString strVersionText = "";

    // name, short description and GPL hint
    if ( bWithHtml )
    {
        strVersionText += "<center><b>";
    }
    else
    {
        strVersionText += " *** ";
    }

    strVersionText += APP_NAME + tr ( ", Version " ) + VERSION;

    if ( bWithHtml )
    {
        strVersionText += "</b><br>";
    }
    else
    {
        strVersionText += "\n *** ";
    }

    strVersionText += tr ( "Internet Jam Session Software" );

    if ( bWithHtml )
    {
        strVersionText += "<br>";
    }
    else
    {
        strVersionText += "\n *** ";
    }

    strVersionText += tr ( "Under the GNU General Public License (GPL)" );

    if ( bWithHtml )
    {
        strVersionText += "</center>";
    }

    return strVersionText;
}


// Help menu -------------------------------------------------------------------
CHelpMenu::CHelpMenu ( QWidget* parent ) : QMenu ( "&?", parent )
{
    // standard help menu consists of about and what's this help
    addAction ( tr ( "What's &This" ), this,
        SLOT ( OnHelpWhatsThis() ), QKeySequence ( Qt::SHIFT + Qt::Key_F1 ) );

    addSeparator();
    addAction ( tr ( "&Download Link..." ), this,
        SLOT ( OnHelpDownloadLink() ) );

    addSeparator();
    addAction ( tr ( "&About..." ), this, SLOT ( OnHelpAbout() ) );
}


/******************************************************************************\
* Other Classes                                                                *
\******************************************************************************/
// Network utility functions ---------------------------------------------------
bool NetworkUtil::ParseNetworkAddress ( QString       strAddress,
                                        CHostAddress& HostAddress )
{
    QHostAddress InetAddr;
    quint16      iNetPort = LLCON_DEFAULT_PORT_NUMBER;

    // init requested host address with invalid address first
    HostAddress = CHostAddress();

    // parse input address for the type [IP address]:[port number]
    QString strPort = strAddress.section ( ":", 1, 1 );
    if ( !strPort.isEmpty() )
    {
        // a colon is present in the address string, try to extract port number
        iNetPort = strPort.toInt();

        // extract address port before colon (should be actual internet address)
        strAddress = strAddress.section ( ":", 0, 0 );
    }

    // first try if this is an IP number an can directly applied to QHostAddress
    if ( !InetAddr.setAddress ( strAddress ) )
    {
        // it was no vaild IP address, try to get host by name, assuming
        // that the string contains a valid host name string
        const QHostInfo HostInfo = QHostInfo::fromName ( strAddress );

        if ( HostInfo.error() == QHostInfo::NoError )
        {
            // apply IP address to QT object
             if ( !HostInfo.addresses().isEmpty() )
             {
                 // use the first IP address
                 InetAddr = HostInfo.addresses().first();
             }
        }
        else
        {
            return false; // invalid address
        }
    }

    HostAddress = CHostAddress ( InetAddr, iNetPort );

    return true;
}


// Instrument picture data base ------------------------------------------------
CVector<CInstPictures::CInstPictProps>& CInstPictures::GetTable()
{
    // make sure we generate the table only once
    static bool TableIsInitialized = false;

    static CVector<CInstPictProps> vecDataBase;

    if ( !TableIsInitialized )
    {
        // instrument picture data base initialization
        // NOTE: Do not change the order of any instrument in the future!
        // NOTE: The very first entry is the "not used" element per definition.
        vecDataBase.Add ( CInstPictProps ( "None", ":/png/instr/res/instrnone.png", IC_OTHER_INSTRUMENT ) ); // special first element
        vecDataBase.Add ( CInstPictProps ( "Drum Set", ":/png/instr/res/instrdrumset.png", IC_PERCUSSION_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Djembe", ":/png/instr/res/instrdjembe.png", IC_PERCUSSION_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Electric Guitar", ":/png/instr/res/instreguitar.png", IC_PLUCKING_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Acoutic Guitar", ":/png/instr/res/instraguitar.png", IC_PLUCKING_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Bass Guitar", ":/png/instr/res/instrbassguitar.png", IC_PLUCKING_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Keyboard", ":/png/instr/res/instrkeyboard.png", IC_KEYBOARD_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Synthesizer", ":/png/instr/res/instrsynthesizer.png", IC_KEYBOARD_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Grand Piano", ":/png/instr/res/instrgrandpiano.png", IC_KEYBOARD_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Accordeon", ":/png/instr/res/instraccordeon.png", IC_KEYBOARD_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Vocal", ":/png/instr/res/instrvocal.png", IC_OTHER_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Microphone", ":/png/instr/res/instrmicrophone.png", IC_OTHER_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Harmonica", ":/png/instr/res/instrharmonica.png", IC_WIND_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Trumpet", ":/png/instr/res/instrtrumpet.png", IC_WIND_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Trombone", ":/png/instr/res/instrtrombone.png", IC_WIND_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "French Horn", ":/png/instr/res/instrfrenchhorn.png", IC_WIND_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Tuba", ":/png/instr/res/instrtuba.png", IC_WIND_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Saxophone", ":/png/instr/res/instrsaxophone.png", IC_WIND_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Clarinet", ":/png/instr/res/instrclarinet.png", IC_WIND_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Flute", ":/png/instr/res/instrflute.png", IC_WIND_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Violin", ":/png/instr/res/instrviolin.png", IC_STRING_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Cello", ":/png/instr/res/instrcello.png", IC_STRING_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Double Bass", ":/png/instr/res/instrdoublebass.png", IC_STRING_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Recorder", ":/png/instr/res/instrrecorder.png", IC_OTHER_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Streamer", ":/png/instr/res/instrstreamer.png", IC_OTHER_INSTRUMENT ) );
        vecDataBase.Add ( CInstPictProps ( "Listener", ":/png/instr/res/instrlistener.png", IC_OTHER_INSTRUMENT ) );

        // now the table is initialized
        TableIsInitialized = true;
    }

    return vecDataBase;
}

bool CInstPictures::IsInstIndexInRange ( const int iIdx )
{
    // check if index is in valid range
    return ( iIdx >= 0 ) && ( iIdx < GetTable().Size() );
}

QString CInstPictures::GetResourceReference ( const int iInstrument )
{
    // range check
    if ( IsInstIndexInRange ( iInstrument ) )
    {
        // return the string of the resource reference for accessing the picture
        return GetTable()[iInstrument].strResourceReference;
    }
    else
    {
        return "";
    }
}

QString CInstPictures::GetName ( const int iInstrument )
{
    // range check
    if ( IsInstIndexInRange ( iInstrument ) )
    {
        // return the name of the instrument
        return GetTable()[iInstrument].strName;
    }
    else
    {
        return "";
    }
}



/******************************************************************************\
* Global Functions Implementation                                              *
\******************************************************************************/
void DebugError ( const QString& pchErDescr,
                  const QString& pchPar1Descr, 
                  const double   dPar1,
                  const QString& pchPar2Descr,
                  const double   dPar2 )
{
    QFile File ( "DebugError.dat" );
    if ( File.open ( QIODevice::Append ) )
    {
        // append new line in logging file
        QTextStream out ( &File );
        out << pchErDescr << " ### " <<
            pchPar1Descr << ": " << QString().setNum ( dPar1, 'f', 2 ) <<
            " ### " <<
            pchPar2Descr << ": " << QString().setNum ( dPar2, 'f', 2 ) <<
            endl;

        File.close();
    }
    printf ( "\nDebug error! For more information see test/DebugError.dat\n" );
    exit ( 1 );
}
