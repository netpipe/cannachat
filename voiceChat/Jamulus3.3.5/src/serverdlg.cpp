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

#include "serverdlg.h"


/* Implementation *************************************************************/
CServerDlg::CServerDlg ( CServer*        pNServP,
                         CSettings*      pNSetP,
                         const bool      bStartMinimized,
                         QWidget*        parent,
                         Qt::WindowFlags f )
    : QDialog                  ( parent, f ),
      pServer                  ( pNServP ),
      pSettings                ( pNSetP ),
      BitmapSystemTrayInactive ( QString::fromUtf8 ( ":/png/LEDs/res/CLEDGreyArrow.png" ) ),
      BitmapSystemTrayActive   ( QString::fromUtf8 ( ":/png/LEDs/res/CLEDGreenArrow.png" ) )
{
    setupUi ( this );


    // Add help text to controls -----------------------------------------------
    // client list
    lvwClients->setWhatsThis ( tr ( "<b>Client List:</b> The client list "
        "shows all clients which are currently connected to this server. Some "
        "informations about the clients like the IP address and name are given "
        "for each connected client." ) );

    lvwClients->setAccessibleName ( tr ( "Connected clients list view" ) );

    // start minimized on operating system start
    chbStartOnOSStart->setWhatsThis ( tr ( "<b>Start Minimized on Operating "
        "System Start:</b> If the start minimized on operating system start "
        "check box is checked, the " ) + APP_NAME + tr ( " server will be "
        "started when the operating system starts up and is automatically "
        "minimized to a system task bar icon." ) );

    // register server flag
    chbRegisterServer->setWhatsThis ( tr ( "<b>Register Server Status:</b> If "
        "the register server check box is checked, this server registers "
        "itself at the central server so that all " ) + APP_NAME +
        tr ( " users can see the server in the connect dialog server list and "
        "connect to it. The registering of the server is renewed periodically "
        "to make sure that all servers in the connect dialog server list are "
        "actually available." ) );

    // central server address
    QString strCentrServAddr = tr ( "<b>Central Server Address:</b> The "
        "Central server address is the IP address or URL of the central server "
        "at which this server is registered. If the Default check box is "
        "checked, the default central server address is shown read-only." );

    lblCentralServerAddress->setWhatsThis ( strCentrServAddr );
    chbDefaultCentralServer->setWhatsThis ( strCentrServAddr );
    edtCentralServerAddress->setWhatsThis ( strCentrServAddr );

    chbDefaultCentralServer->setAccessibleName (
        tr ( "Default central server check box" ) );

    edtCentralServerAddress->setAccessibleName (
        tr ( "Central server address line edit" ) );

    // server name
    QString strServName = tr ( "<b>Server Name:</b> The server name identifies "
        "your server in the connect dialog server list at the clients. If no "
        "name is given, the IP address is shown instead." );

    lblServerName->setWhatsThis ( strServName );
    edtServerName->setWhatsThis ( strServName );

    edtServerName->setAccessibleName ( tr ( "Server name line edit" ) );

    // location city
    QString strLocCity = tr ( "<b>Location City:</b> The city in which this "
        "server is located can be set here. If a city name is entered, it "
        "will be shown in the connect dialog server list at the clients." );

    lblLocationCity->setWhatsThis ( strLocCity );
    edtLocationCity->setWhatsThis ( strLocCity );

    edtLocationCity->setAccessibleName ( tr (
        "City where the server is located line edit" ) );

    // location country
    QString strLocCountry = tr ( "<b>Location country:</b> The country in "
        "which this server is located can be set here. If a country is "
        "entered, it will be shown in the connect dialog server list at the "
        "clients." );

    lblLocationCountry->setWhatsThis ( strLocCountry );
    cbxLocationCountry->setWhatsThis ( strLocCountry );

    cbxLocationCountry->setAccessibleName ( tr (
        "Country where the server is located combo box" ) );


    // check if system tray icon can be used
    bSystemTrayIconAvaialbe = SystemTrayIcon.isSystemTrayAvailable();

    // init system tray icon
    if ( bSystemTrayIconAvaialbe )
    {
        // prepare context menu to be added to the system tray icon
        pSystemTrayIconMenu = new QMenu ( this );

        pSystemTrayIconMenu->addAction ( tr ( "E&xit" ),
            this, SLOT ( OnSysTrayMenuExit() ) );

        pSystemTrayIconMenu->addSeparator();

        pSystemTrayIconMenu->addAction (
            tr ( "&Hide " ) + APP_NAME + tr ( " server" ),
            this, SLOT ( OnSysTrayMenuHide() ) );

        pSystemTrayIconMenu->setDefaultAction ( pSystemTrayIconMenu->addAction (
            tr ( "&Open " ) + APP_NAME + tr ( " server" ),
            this, SLOT ( OnSysTrayMenuOpen() ) ) );

        SystemTrayIcon.setContextMenu ( pSystemTrayIconMenu );

        // set tool text
        SystemTrayIcon.setToolTip ( QString ( APP_NAME ) + tr ( " server" ) );

        // show icon of state "inactive"
        SystemTrayIcon.setIcon ( QIcon ( BitmapSystemTrayInactive ) );
        SystemTrayIcon.show();
    }

    // act on "start minimized" flag (note, this has to be done after setting
    // the correct value for the system tray icon availablility)
    if ( bStartMinimized )
    {
        showMinimized();
    }

    // set text for version and application name
    lblNameVersion->setText ( QString ( APP_NAME ) +
        tr ( " server " ) + QString ( VERSION ) );

    // set up list view for connected clients
    lvwClients->setColumnWidth ( 0, 170 );
    lvwClients->setColumnWidth ( 1, 130 );
    lvwClients->clear();


// TEST workaround for resize problem of window after iconize in task bar
lvwClients->setMinimumWidth ( 170 + 130 + 60 + 205 );
lvwClients->setMinimumHeight ( 140 );


    // insert items in reverse order because in Windows all of them are
    // always visible -> put first item on the top
    vecpListViewItems.Init ( MAX_NUM_CHANNELS );
    for ( int i = MAX_NUM_CHANNELS - 1; i >= 0; i-- )
    {
        vecpListViewItems[i] = new QTreeWidgetItem ( lvwClients );
        vecpListViewItems[i]->setHidden ( true );
    }

    // update default central server address check box
    if ( pServer->GetUseDefaultCentralServerAddress() )
    {
        chbDefaultCentralServer->setCheckState ( Qt::Checked );
    }
    else
    {
        chbDefaultCentralServer->setCheckState ( Qt::Unchecked );
    }

    // update server name line edit
    edtServerName->setText ( pServer->GetServerName() );

    // update server city line edit
    edtLocationCity->setText ( pServer->GetServerCity() );

    // load country combo box with all available countries
    cbxLocationCountry->setInsertPolicy ( QComboBox::NoInsert );
    cbxLocationCountry->clear();

    for ( int iCurCntry = static_cast<int> ( QLocale::AnyCountry );
          iCurCntry < static_cast<int> ( QLocale::LastCountry ); iCurCntry++ )
    {
        // add all countries except of the "Default" country
        if ( static_cast<QLocale::Country> ( iCurCntry ) != QLocale::AnyCountry )
        {
            // store the country enum index together with the string (this is
            // important since we sort the combo box items later on)
            cbxLocationCountry->addItem ( QLocale::countryToString (
                static_cast<QLocale::Country> ( iCurCntry ) ), iCurCntry );
        }
    }

    // sort country combo box items in alphabetical order
    cbxLocationCountry->model()->sort ( 0, Qt::AscendingOrder );

    // select current country
    cbxLocationCountry->setCurrentIndex (
        cbxLocationCountry->findData (
        static_cast<int> ( pServer->GetServerCountry() ) ) );

    // update register server check box
    if ( pServer->GetServerListEnabled() )
    {
        chbRegisterServer->setCheckState ( Qt::Checked );
    }
    else
    {
        chbRegisterServer->setCheckState ( Qt::Unchecked );
    }

    // update start minimized check box (only available for Windows)
#ifndef _WIN32
    chbStartOnOSStart->setVisible ( false );
#else
    const bool bCurAutoStartMinState = pServer->GetAutoRunMinimized();

    if ( bCurAutoStartMinState )
    {
        chbStartOnOSStart->setCheckState ( Qt::Checked );
    }
    else
    {
        chbStartOnOSStart->setCheckState ( Qt::Unchecked );
    }

    // modify registry according to setting (this is just required in case a
    // user has changed the registry by hand)
    ModifyAutoStartEntry ( bCurAutoStartMinState );
#endif

    // update GUI dependencies
    UpdateGUIDependencies();

    // set window title
    setWindowTitle ( APP_NAME + tr ( " Server" ) );


    // View menu  --------------------------------------------------------------
    QMenu* pViewMenu = new QMenu ( "&Window", this );

    pViewMenu->addAction ( tr ( "E&xit" ), this,
        SLOT ( close() ), QKeySequence ( Qt::CTRL + Qt::Key_Q ) );


    // Main menu bar -----------------------------------------------------------
    pMenu = new QMenuBar ( this );

    pMenu->addMenu ( pViewMenu );
    pMenu->addMenu ( new CHelpMenu ( this ) );

    // Now tell the layout about the menu
    layout()->setMenuBar ( pMenu );


    // Connections -------------------------------------------------------------
    // check boxes
    QObject::connect ( chbRegisterServer, SIGNAL ( stateChanged ( int ) ),
        this, SLOT ( OnRegisterServerStateChanged ( int ) ) );

    QObject::connect ( chbDefaultCentralServer, SIGNAL ( stateChanged ( int ) ),
        this, SLOT ( OnDefaultCentralServerStateChanged ( int ) ) );

    QObject::connect ( chbStartOnOSStart, SIGNAL ( stateChanged ( int ) ),
        this, SLOT ( OnStartOnOSStartStateChanged ( int ) ) );

    // line edits
    QObject::connect ( edtCentralServerAddress, SIGNAL ( editingFinished() ),
        this, SLOT ( OnCentralServerAddressEditingFinished() ) );

    QObject::connect ( edtServerName, SIGNAL ( textChanged ( const QString& ) ),
        this, SLOT ( OnServerNameTextChanged ( const QString& ) ) );

    QObject::connect ( edtLocationCity, SIGNAL ( textChanged ( const QString& ) ),
        this, SLOT ( OnLocationCityTextChanged ( const QString& ) ) );

    // combo boxes
    QObject::connect ( cbxLocationCountry, SIGNAL ( activated ( int ) ),
        this, SLOT ( OnLocationCountryActivated ( int ) ) );

    // timers
    QObject::connect ( &Timer, SIGNAL ( timeout() ), this, SLOT ( OnTimer() ) );

    // other
    QObject::connect ( pServer, SIGNAL ( Started() ),
        this, SLOT ( OnServerStarted() ) );

    QObject::connect ( pServer, SIGNAL ( Stopped() ),
        this, SLOT ( OnServerStopped() ) );

    QObject::connect ( QCoreApplication::instance(), SIGNAL ( aboutToQuit() ),
        this, SLOT ( OnAboutToQuit() ) );

    QObject::connect ( &SystemTrayIcon,
        SIGNAL ( activated ( QSystemTrayIcon::ActivationReason ) ),
        this, SLOT ( OnSysTrayActivated ( QSystemTrayIcon::ActivationReason ) ) );


    // Timers ------------------------------------------------------------------
    // start timer for GUI controls
    Timer.start ( GUI_CONTRL_UPDATE_TIME );
}

void CServerDlg::closeEvent ( QCloseEvent* Event )
{
    // if server was registered at the central server, unregister on shutdown
    if ( pServer->GetServerListEnabled() )
    {
        pServer->UnregisterSlaveServer();
    }

    // default implementation of this event handler routine
    Event->accept();
}

void CServerDlg::OnStartOnOSStartStateChanged ( int value )
{
    const bool bCurAutoStartMinState = ( value == Qt::Checked );

    // update registry and server setting (for ini file)
    pServer->SetAutoRunMinimized ( bCurAutoStartMinState );
    ModifyAutoStartEntry         ( bCurAutoStartMinState );
}

void CServerDlg::OnDefaultCentralServerStateChanged ( int value )
{
    // apply new setting to the server and update it
    pServer->SetUseDefaultCentralServerAddress ( value == Qt::Checked );
    pServer->UpdateServerList();

    // update GUI dependencies
    UpdateGUIDependencies();
}

void CServerDlg::OnRegisterServerStateChanged ( int value )
{
    const bool bRegState = ( value == Qt::Checked );

    // apply new setting to the server and update it
    pServer->SetServerListEnabled ( bRegState );

    // if registering is disabled, unregister slave server
    if ( !bRegState )
    {
        pServer->UnregisterSlaveServer();
    }

    pServer->UpdateServerList();

    // update GUI dependencies
    UpdateGUIDependencies();
}

void CServerDlg::OnCentralServerAddressEditingFinished()
{
    // apply new setting to the server and update it
    pServer->SetServerListCentralServerAddress (
        edtCentralServerAddress->text() );

    pServer->UpdateServerList();
}

void CServerDlg::OnServerNameTextChanged ( const QString& strNewName )
{
    // check length
    if ( strNewName.length() <= MAX_LEN_SERVER_NAME )
    {
        // apply new setting to the server and update it
        pServer->SetServerName ( strNewName );
        pServer->UpdateServerList();
    }
    else
    {
        // text is too long, update control with shortend text
        edtServerName->setText ( strNewName.left ( MAX_LEN_SERVER_NAME ) );
    }
}

void CServerDlg::OnLocationCityTextChanged ( const QString& strNewCity )
{
    // check length
    if ( strNewCity.length() <= MAX_LEN_SERVER_CITY )
    {
        // apply new setting to the server and update it
        pServer->SetServerCity ( strNewCity );
        pServer->UpdateServerList();
    }
    else
    {
        // text is too long, update control with shortend text
        edtLocationCity->setText ( strNewCity.left ( MAX_LEN_SERVER_CITY ) );
    }
}

void CServerDlg::OnLocationCountryActivated ( int iCntryListItem )
{
    // apply new setting to the server and update it
    pServer->SetServerCountry ( static_cast<QLocale::Country> (
        cbxLocationCountry->itemData ( iCntryListItem ).toInt() ) );

    pServer->UpdateServerList();
}

void CServerDlg::OnSysTrayActivated ( QSystemTrayIcon::ActivationReason ActReason )
{
    // on double click on the icon, show window in fore ground
    if ( ActReason == QSystemTrayIcon::DoubleClick )
    {
        ShowWindowInForeground();
    }
}

void CServerDlg::OnTimer()
{
    CVector<CHostAddress> vecHostAddresses;
    CVector<QString>      vecsName;
    CVector<int>          veciJitBufNumFrames;
    CVector<int>          veciNetwFrameSizeFact;

    ListViewMutex.lock();
    {
        pServer->GetConCliParam ( vecHostAddresses,
                                  vecsName,
                                  veciJitBufNumFrames,
                                  veciNetwFrameSizeFact );

        // we assume that all vectors have the same length
        const int iNumChannels = vecHostAddresses.Size();

        // fill list with connected clients
        for ( int i = 0; i < iNumChannels; i++ )
        {
            if ( !( vecHostAddresses[i].InetAddr == QHostAddress ( (quint32) 0 ) ) )
            {
                // IP, port number
                vecpListViewItems[i]->setText ( 0,
                    vecHostAddresses[i].toString ( CHostAddress::SM_IP_PORT ) );

                // name
                vecpListViewItems[i]->setText ( 1, vecsName[i] );

                // jitter buffer size (polling for updates)
                vecpListViewItems[i]->setText ( 2,
                    QString().setNum ( veciJitBufNumFrames[i] ) );

                // out network block size
                vecpListViewItems[i]->setText ( 3,
                    QString().setNum ( static_cast<double> (
                    veciNetwFrameSizeFact[i] * SYSTEM_BLOCK_DURATION_MS_FLOAT
                    ), 'f', 2 ) );

                vecpListViewItems[i]->setHidden ( false );
            }
            else
            {
                vecpListViewItems[i]->setHidden ( true );
            }
        }
    }
    ListViewMutex.unlock();
}

void CServerDlg::UpdateGUIDependencies()
{
    // get the states which define the GUI dependencies from the server
    const bool bCurSerListEnabled = pServer->GetServerListEnabled();

    const bool bCurUseDefCentServAddr =
        pServer->GetUseDefaultCentralServerAddress();

    // if register server is not enabled, we disable all the configuration
    // controls for the server list
    chbDefaultCentralServer->setEnabled ( bCurSerListEnabled );
    grbServerInfo->setEnabled           ( bCurSerListEnabled );

    // If the default central server address is enabled, the line edit shows
    // the default server and is not editable. Make sure the line edit does not
    // fire signals when we update the text.
    edtCentralServerAddress->blockSignals ( true );
    {
        edtCentralServerAddress->setText (
            SELECT_SERVER_ADDRESS ( bCurUseDefCentServAddr,
                                    pServer->GetServerListCentralServerAddress() ) );
    }
    edtCentralServerAddress->blockSignals ( false );

    // the line edit of the central server address is only enabled, if the
    // server list is enabled and not the default address is used
    edtCentralServerAddress->setEnabled (
        !bCurUseDefCentServAddr && bCurSerListEnabled );
}

void CServerDlg::UpdateSystemTrayIcon ( const bool bIsActive )
{
    if ( bSystemTrayIconAvaialbe )
    {
        if ( bIsActive )
        {
            SystemTrayIcon.setIcon ( QIcon ( BitmapSystemTrayActive ) );
        }
        else
        {
            SystemTrayIcon.setIcon ( QIcon ( BitmapSystemTrayInactive ) );
        }
    }
}

void CServerDlg::ModifyAutoStartEntry ( const bool bDoAutoStart )
{
// auto start is currently only supported for Windows
#ifdef _WIN32
    // init settings object so that it points to the correct place in the
    // Windows registry for the auto run entry
    QSettings RegSettings ( "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                            QSettings::NativeFormat );

    // create start string of auto run entry
    QString strRegValue =
        QCoreApplication::applicationFilePath().replace ( "/", "\\" ) +
        " -s --startminimized";
#endif

    if ( bDoAutoStart )
    {
#ifdef _WIN32
        // ckeck if registry entry is correctly present, if not, correct
        const bool bWriteRegValue = strRegValue.compare (
            RegSettings.value ( AUTORUN_SERVER_REG_NAME ).toString() );

        if ( bWriteRegValue )
        {
            // write reg key in the registry
            RegSettings.setValue ( AUTORUN_SERVER_REG_NAME, strRegValue );
        }
#endif
    }
    else
    {
#ifdef _WIN32
        // delete reg key if present
        if ( RegSettings.contains ( AUTORUN_SERVER_REG_NAME ) )
        {
            RegSettings.remove ( AUTORUN_SERVER_REG_NAME );
        }
#endif
    }
}

void CServerDlg::changeEvent ( QEvent* pEvent )
{
    // if we have a system tray icon, we make the window invisible if it is
    // minimized
    if ( bSystemTrayIconAvaialbe &&
         ( pEvent->type() == QEvent::WindowStateChange ) )
    {
        if ( isMinimized() )
        {
            // we have to call the hide function from another thread -> use
            // the timer for this purpose
            QTimer::singleShot ( 0, this, SLOT ( hide() ) );
        }
    }
}
