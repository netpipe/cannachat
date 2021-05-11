VERSION = 3.3.5

# use target name which does not use a captital letter at the beginning
contains(CONFIG, "noupcasename") {
    message(The target name is jamulus instead of Jamulus.)
    TARGET = jamulus
}

CONFIG += qt \
    thread \
    release

QT += widgets \
    network \
    xml

INCLUDEPATH += src

INCLUDEPATH_CELT = libs/celt

INCLUDEPATH_OPUS = libs/opus/include \
    libs/opus/celt \
    libs/opus/silk \
    libs/opus/silk/float

DEFINES += APP_VERSION=\\\"$$VERSION\\\" \
    OPUS_BUILD \
    USE_ALLOCA \
    CUSTOM_MODES \
    _REENTRANT

win32 {
    DEFINES -= UNICODE # fixes issue with ASIO SDK (asiolist.cpp is not unicode compatible)
    DEFINES += NOMINMAX # solves a compiler error in qdatetime.h (Qt5)
    HEADERS += windows/sound.h
    SOURCES += windows/sound.cpp \
        windows/ASIOSDK2/common/asio.cpp \
        windows/ASIOSDK2/host/asiodrivers.cpp \
        windows/ASIOSDK2/host/pc/asiolist.cpp
    RC_FILE = windows/mainicon.rc
    INCLUDEPATH += windows/ASIOSDK2/common \
        windows/ASIOSDK2/host \
        windows/ASIOSDK2/host/pc
    LIBS += ole32.lib \
        user32.lib \
        advapi32.lib \
        winmm.lib \
        ws2_32.lib
} else:macx {
    HEADERS += mac/sound.h
    SOURCES += mac/sound.cpp
    RC_FILE = mac/mainicon.icns
    CONFIG += x86

    LIBS += -framework CoreFoundation \
        -framework CoreServices \
        -framework CoreAudio \
        -framework AudioToolbox \
        -framework AudioUnit
} else:android {
    HEADERS += android/sound.h
    SOURCES += android/sound.cpp
    LIBS += -lOpenSLES
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
    OTHER_FILES += android/AndroidManifest.xml
} else:unix {
    # we assume that stdint.h is always present in a Linux system
    DEFINES += HAVE_STDINT_H

    # only include jack support if CONFIG nosound is not set
    nosoundoption = $$find(CONFIG, "nosound")
    count(nosoundoption, 0) {
        message(Jack Audio Interface Enabled.)

        !exists(/usr/include/jack/jack.h) {
            !exists(/usr/local/include/jack/jack.h) {
                 message(Warning: jack.h was not found at the usual place, maybe the jack dev packet is missing)
            }
        }

        HEADERS += linux/sound.h
        SOURCES += linux/sound.cpp
        DEFINES += WITH_SOUND
        LIBS += -ljack
    }

    # Linux is our source distribution, include sources from other OSs
    DISTFILES += mac/sound.h \
        mac/sound.cpp \
        mac/mainicon.icns \
        windows/sound.h \
        windows/sound.cpp \
        windows/mainicon.rc \
        windows/mainicon.ico \
        android/AndroidManifest.xml \
        android/sound.h \
        android/sound.cpp
}

RCC_DIR = src/res
RESOURCES += src/resources.qrc

FORMS += src/clientdlgbase.ui \
    src/serverdlgbase.ui \
    src/clientsettingsdlgbase.ui \
    src/chatdlgbase.ui \
    src/connectdlgbase.ui \
    src/aboutdlgbase.ui

HEADERS += src/audiomixerboard.h \
    src/buffer.h \
    src/channel.h \
    src/chatdlg.h \
    src/client.h \
    src/clientsettingsdlg.h \
    src/connectdlg.h \
    src/global.h \
    src/clientdlg.h \
    src/serverdlg.h \
    src/multicolorled.h \
    src/multicolorledbar.h \
    src/protocol.h \
    src/server.h \
    src/serverlist.h \
    src/serverlogging.h \
    src/settings.h \
    src/socket.h \
    src/soundbase.h \
    src/testbench.h \
    src/util.h \
    src/analyzerconsole.h

HEADERS_CELT = libs/celt/cc6_celt.h \
    libs/celt/cc6_celt_types.h \
    libs/celt/cc6_celt_header.h \
    libs/celt/cc6__kiss_fft_guts.h \
    libs/celt/cc6_arch.h \
    libs/celt/cc6_bands.h \
    libs/celt/cc6_fixed_c5x.h \
    libs/celt/cc6_fixed_c6x.h \
    libs/celt/cc6_cwrs.h \
    libs/celt/cc6_ecintrin.h \
    libs/celt/cc6_entcode.h \
    libs/celt/cc6_entdec.h \
    libs/celt/cc6_entenc.h \
    libs/celt/cc6_fixed_generic.h \
    libs/celt/cc6_float_cast.h \
    libs/celt/cc6_kfft_double.h \
    libs/celt/cc6_kfft_single.h \
    libs/celt/cc6_kiss_fft.h \
    libs/celt/cc6_kiss_fftr.h \
    libs/celt/cc6_laplace.h \
    libs/celt/cc6_mdct.h \
    libs/celt/cc6_mfrngcod.h \
    libs/celt/cc6_mathops.h \
    libs/celt/cc6_modes.h \
    libs/celt/cc6_os_support.h \
    libs/celt/cc6_pitch.h \
    libs/celt/cc6_psy.h \
    libs/celt/cc6_quant_bands.h \
    libs/celt/cc6_rate.h \
    libs/celt/cc6_stack_alloc.h \
    libs/celt/cc6_vq.h

HEADERS_OPUS = libs/opus/include/opus.h \
    libs/opus/include/opus_multistream.h \
    libs/opus/include/opus_custom.h \
    libs/opus/include/opus_types.h \
    libs/opus/include/opus_defines.h \
    libs/opus/src/opus_private.h \
    libs/opus/src/analysis.h \
    libs/opus/src/mlp.h \
    libs/opus/src/tansig_table.h \
    libs/opus/celt/arch.h \
    libs/opus/celt/bands.h \
    libs/opus/celt/celt.h \
    libs/opus/celt/cpu_support.h \
    libs/opus/celt/cwrs.h \
    libs/opus/celt/ecintrin.h \
    libs/opus/celt/entcode.h \
    libs/opus/celt/entdec.h \
    libs/opus/celt/entenc.h \
    libs/opus/celt/fixed_debug.h \
    libs/opus/celt/fixed_generic.h \
    libs/opus/celt/float_cast.h \
    libs/opus/celt/_kiss_fft_guts.h \
    libs/opus/celt/kiss_fft.h \
    libs/opus/celt/laplace.h \
    libs/opus/celt/mathops.h \
    libs/opus/celt/mdct.h \
    libs/opus/celt/mfrngcod.h \
    libs/opus/celt/modes.h \
    libs/opus/celt/os_support.h \
    libs/opus/celt/pitch.h \
    libs/opus/celt/celt_lpc.h \
    libs/opus/celt/quant_bands.h \
    libs/opus/celt/rate.h \
    libs/opus/celt/stack_alloc.h \
    libs/opus/celt/vq.h \
    libs/opus/celt/static_modes_float.h \
    libs/opus/celt/static_modes_fixed.h \
    libs/opus/celt/arm/armcpu.h \
    libs/opus/celt/arm/fixed_armv4.h \
    libs/opus/celt/arm/fixed_armv5e.h \
    libs/opus/celt/arm/kiss_fft_armv4.h \
    libs/opus/celt/arm/kiss_fft_armv5e.h \
    libs/opus/celt/arm/pitch_arm.h \
    libs/opus/celt/x86/pitch_sse.h \
    libs/opus/silk/debug.h \
    libs/opus/silk/control.h \
    libs/opus/silk/errors.h \
    libs/opus/silk/API.h \
    libs/opus/silk/typedef.h \
    libs/opus/silk/define.h \
    libs/opus/silk/main.h \
    libs/opus/silk/PLC.h \
    libs/opus/silk/SigProc_FIX.h \
    libs/opus/silk/arm/macros_armv4.h \
    libs/opus/silk/arm/macros_armv5e.h \
    libs/opus/silk/arm/SigProc_FIX_armv4.h \
    libs/opus/silk/arm/SigProc_FIX_armv5e.h \
    libs/opus/silk/structs.h \
    libs/opus/silk/tables.h \
    libs/opus/silk/tuning_parameters.h \
    libs/opus/silk/Inlines.h \
    libs/opus/silk/MacroCount.h \
    libs/opus/silk/MacroDebug.h \
    libs/opus/silk/macros.h \
    libs/opus/silk/pitch_est_defines.h \
    libs/opus/silk/resampler_private.h \
    libs/opus/silk/resampler_rom.h \
    libs/opus/silk/resampler_structs.h \
    libs/opus/silk/float/main_FLP.h \
    libs/opus/silk/float/structs_FLP.h \
    libs/opus/silk/float/SigProc_FLP.h

SOURCES += src/audiomixerboard.cpp \
    src/buffer.cpp \
    src/channel.cpp \
    src/chatdlg.cpp \
    src/client.cpp \
    src/clientsettingsdlg.cpp \
    src/connectdlg.cpp \
    src/clientdlg.cpp \
    src/serverdlg.cpp \
    src/main.cpp \
    src/multicolorled.cpp \
    src/multicolorledbar.cpp \
    src/protocol.cpp \
    src/server.cpp \
    src/serverlist.cpp \
    src/serverlogging.cpp \
    src/settings.cpp \
    src/socket.cpp \
    src/soundbase.cpp \
    src/util.cpp \
    src/analyzerconsole.cpp

SOURCES_CELT = libs/celt/cc6_bands.c \
    libs/celt/cc6_celt.c \
    libs/celt/cc6_cwrs.c \
    libs/celt/cc6_entcode.c \
    libs/celt/cc6_entdec.c \
    libs/celt/cc6_entenc.c \
    libs/celt/cc6_header.c \
    libs/celt/cc6_kfft_single.c \
    libs/celt/cc6__kiss_fft.c \
    libs/celt/cc6__kiss_fftr.c \
    libs/celt/cc6_laplace.c \
    libs/celt/cc6_mdct.c \
    libs/celt/cc6_modes.c \
    libs/celt/cc6_pitch.c \
    libs/celt/cc6_psy.c \
    libs/celt/cc6_quant_bands.c \
    libs/celt/cc6_rangedec.c \
    libs/celt/cc6_rangeenc.c \
    libs/celt/cc6_rate.c \
    libs/celt/cc6_vq.c

SOURCES_OPUS = libs/opus/src/opus.c \
    libs/opus/src/opus_decoder.c \
    libs/opus/src/opus_encoder.c \
    libs/opus/src/opus_multistream.c \
    libs/opus/src/opus_multistream_encoder.c \
    libs/opus/src/opus_multistream_decoder.c \
    libs/opus/src/repacketizer.c \
    libs/opus/src/analysis.c \
    libs/opus/src/mlp.c \
    libs/opus/src/mlp_data.c \
    libs/opus/celt/bands.c \
    libs/opus/celt/celt.c \
    libs/opus/celt/celt_encoder.c \
    libs/opus/celt/celt_decoder.c \
    libs/opus/celt/cwrs.c \
    libs/opus/celt/entcode.c \
    libs/opus/celt/entdec.c \
    libs/opus/celt/entenc.c \
    libs/opus/celt/kiss_fft.c \
    libs/opus/celt/laplace.c \
    libs/opus/celt/mathops.c \
    libs/opus/celt/mdct.c \
    libs/opus/celt/modes.c \
    libs/opus/celt/pitch.c \
    libs/opus/celt/celt_lpc.c \
    libs/opus/celt/quant_bands.c \
    libs/opus/celt/rate.c \
    libs/opus/celt/vq.c \
    libs/opus/celt/arm/armcpu.c \
    libs/opus/celt/arm/arm_celt_map.c \
    libs/opus/silk/CNG.c \
    libs/opus/silk/code_signs.c \
    libs/opus/silk/init_decoder.c \
    libs/opus/silk/decode_core.c \
    libs/opus/silk/decode_frame.c \
    libs/opus/silk/decode_parameters.c \
    libs/opus/silk/decode_indices.c \
    libs/opus/silk/decode_pulses.c \
    libs/opus/silk/decoder_set_fs.c \
    libs/opus/silk/dec_API.c \
    libs/opus/silk/enc_API.c \
    libs/opus/silk/encode_indices.c \
    libs/opus/silk/encode_pulses.c \
    libs/opus/silk/gain_quant.c \
    libs/opus/silk/interpolate.c \
    libs/opus/silk/LP_variable_cutoff.c \
    libs/opus/silk/NLSF_decode.c \
    libs/opus/silk/NSQ.c \
    libs/opus/silk/NSQ_del_dec.c \
    libs/opus/silk/PLC.c \
    libs/opus/silk/shell_coder.c \
    libs/opus/silk/tables_gain.c \
    libs/opus/silk/tables_LTP.c \
    libs/opus/silk/tables_NLSF_CB_NB_MB.c \
    libs/opus/silk/tables_NLSF_CB_WB.c \
    libs/opus/silk/tables_other.c \
    libs/opus/silk/tables_pitch_lag.c \
    libs/opus/silk/tables_pulses_per_block.c \
    libs/opus/silk/VAD.c \
    libs/opus/silk/control_audio_bandwidth.c \
    libs/opus/silk/quant_LTP_gains.c \
    libs/opus/silk/VQ_WMat_EC.c \
    libs/opus/silk/HP_variable_cutoff.c \
    libs/opus/silk/NLSF_encode.c \
    libs/opus/silk/NLSF_VQ.c \
    libs/opus/silk/NLSF_unpack.c \
    libs/opus/silk/NLSF_del_dec_quant.c \
    libs/opus/silk/process_NLSFs.c \
    libs/opus/silk/stereo_LR_to_MS.c \
    libs/opus/silk/stereo_MS_to_LR.c \
    libs/opus/silk/check_control_input.c \
    libs/opus/silk/control_SNR.c \
    libs/opus/silk/init_encoder.c \
    libs/opus/silk/control_codec.c \
    libs/opus/silk/A2NLSF.c \
    libs/opus/silk/ana_filt_bank_1.c \
    libs/opus/silk/biquad_alt.c \
    libs/opus/silk/bwexpander_32.c \
    libs/opus/silk/bwexpander.c \
    libs/opus/silk/debug.c \
    libs/opus/silk/decode_pitch.c \
    libs/opus/silk/inner_prod_aligned.c \
    libs/opus/silk/lin2log.c \
    libs/opus/silk/log2lin.c \
    libs/opus/silk/LPC_analysis_filter.c \
    libs/opus/silk/LPC_inv_pred_gain.c \
    libs/opus/silk/table_LSF_cos.c \
    libs/opus/silk/NLSF2A.c \
    libs/opus/silk/NLSF_stabilize.c \
    libs/opus/silk/NLSF_VQ_weights_laroia.c \
    libs/opus/silk/pitch_est_tables.c \
    libs/opus/silk/resampler.c \
    libs/opus/silk/resampler_down2_3.c \
    libs/opus/silk/resampler_down2.c \
    libs/opus/silk/resampler_private_AR2.c \
    libs/opus/silk/resampler_private_down_FIR.c \
    libs/opus/silk/resampler_private_IIR_FIR.c \
    libs/opus/silk/resampler_private_up2_HQ.c \
    libs/opus/silk/resampler_rom.c \
    libs/opus/silk/sigm_Q15.c \
    libs/opus/silk/sort.c \
    libs/opus/silk/sum_sqr_shift.c \
    libs/opus/silk/stereo_decode_pred.c \
    libs/opus/silk/stereo_encode_pred.c \
    libs/opus/silk/stereo_find_predictor.c \
    libs/opus/silk/stereo_quant_pred.c \
    libs/opus/silk/float/apply_sine_window_FLP.c \
    libs/opus/silk/float/corrMatrix_FLP.c \
    libs/opus/silk/float/encode_frame_FLP.c \
    libs/opus/silk/float/find_LPC_FLP.c \
    libs/opus/silk/float/find_LTP_FLP.c \
    libs/opus/silk/float/find_pitch_lags_FLP.c \
    libs/opus/silk/float/find_pred_coefs_FLP.c \
    libs/opus/silk/float/LPC_analysis_filter_FLP.c \
    libs/opus/silk/float/LTP_analysis_filter_FLP.c \
    libs/opus/silk/float/LTP_scale_ctrl_FLP.c \
    libs/opus/silk/float/noise_shape_analysis_FLP.c \
    libs/opus/silk/float/prefilter_FLP.c \
    libs/opus/silk/float/process_gains_FLP.c \
    libs/opus/silk/float/regularize_correlations_FLP.c \
    libs/opus/silk/float/residual_energy_FLP.c \
    libs/opus/silk/float/solve_LS_FLP.c \
    libs/opus/silk/float/warped_autocorrelation_FLP.c \
    libs/opus/silk/float/wrappers_FLP.c \
    libs/opus/silk/float/autocorrelation_FLP.c \
    libs/opus/silk/float/burg_modified_FLP.c \
    libs/opus/silk/float/bwexpander_FLP.c \
    libs/opus/silk/float/energy_FLP.c \
    libs/opus/silk/float/inner_product_FLP.c \
    libs/opus/silk/float/k2a_FLP.c \
    libs/opus/silk/float/levinsondurbin_FLP.c \
    libs/opus/silk/float/LPC_inv_pred_gain_FLP.c \
    libs/opus/silk/float/pitch_analysis_core_FLP.c \
    libs/opus/silk/float/scale_copy_vector_FLP.c \
    libs/opus/silk/float/scale_vector_FLP.c \
    libs/opus/silk/float/schur_FLP.c \
    libs/opus/silk/float/sort_FLP.c

DISTFILES += AUTHORS \
    ChangeLog \
    COPYING \
    INSTALL \
    NEWS \
    README \
    TODO \
    src/res/jamulus.desktop \
    src/res/CLEDBlack.png \
    src/res/CLEDBlackSmall.png \
    src/res/CLEDDisabledSmall.png \
    src/res/CLEDGreen.png \
    src/res/CLEDGreenArrow.png \
    src/res/CLEDGreenSmall.png \
    src/res/CLEDGrey.png \
    src/res/CLEDGreyArrow.png \
    src/res/CLEDGreySmall.png \
    src/res/CLEDRed.png \
    src/res/CLEDRedSmall.png \
    src/res/CLEDYellow.png \
    src/res/CLEDYellowSmall.png \
    src/res/faderbackground.png \
    src/res/faderhandle.png \
    src/res/faderhandlesmall.png \
    src/res/HLEDGreen.png \
    src/res/HLEDGreenSmall.png \
    src/res/HLEDGrey.png \
    src/res/HLEDGreySmall.png \
    src/res/HLEDRed.png \
    src/res/HLEDRedSmall.png \
    src/res/HLEDYellow.png \
    src/res/HLEDYellowSmall.png \
    src/res/ledbuttonnotpressed.png \
    src/res/ledbuttonpressed.png \
    src/res/fronticon.png \
    src/res/logopicture.png \
    src/res/mainicon.png \
    src/res/mixerboardbackground.png \
    src/res/VLEDBlack.png \
    src/res/VLEDBlackSmall.png \
    src/res/VLEDDisabledSmall.png \
    src/res/VLEDGreen.png \
    src/res/VLEDGreenSmall.png \
    src/res/VLEDGrey.png \
    src/res/VLEDGreySmall.png \
    src/res/VLEDRed.png \
    src/res/VLEDRedSmall.png \
    src/res/VLEDYellow.png \
    src/res/VLEDYellowSmall.png \
    src/res/VRLEDBlack.png \
    src/res/VRLEDBlackSmall.png \
    src/res/VRLEDGreen.png \
    src/res/VRLEDGreenSmall.png \
    src/res/VRLEDGrey.png \
    src/res/VRLEDGreySmall.png \
    src/res/VRLEDRed.png \
    src/res/VRLEDRedSmall.png \
    src/res/VRLEDYellow.png \
    src/res/VRLEDYellowSmall.png \
    src/res/instraccordeon.png \
    src/res/instraguitar.png \
    src/res/instrbassguitar.png \
    src/res/instrcello.png \
    src/res/instrclarinet.png \
    src/res/instrdjembe.png \
    src/res/instrdoublebass.png \
    src/res/instrdrumset.png \
    src/res/instreguitar.png \
    src/res/instrflute.png \
    src/res/instrfrenchhorn.png \
    src/res/instrgrandpiano.png \
    src/res/instrharmonica.png \
    src/res/instrkeyboard.png \
    src/res/instrlistener.png \
    src/res/instrmicrophone.png \
    src/res/instrnone.png \
    src/res/instrrecorder.png \
    src/res/instrsaxophone.png \
    src/res/instrstreamer.png \
    src/res/instrsynthesizer.png \
    src/res/instrtrombone.png \
    src/res/instrtrumpet.png \
    src/res/instrtuba.png \
    src/res/instrviolin.png \
    src/res/instrvocal.png

DISTFILES_CELT += libs/celt/AUTHORS \
    libs/celt/ChangeLog \
    libs/celt/COPYING \
    libs/celt/INSTALL \
    libs/celt/NEWS \
    libs/celt/README \
    libs/celt/README_LLCON \
    libs/celt/TODO

DISTFILES_OPUS += libs/opus/AUTHORS \
    libs/opus/ChangeLog \
    libs/opus/COPYING \
    libs/opus/INSTALL \
    libs/opus/NEWS \
    libs/opus/README \
    libs/opus/celt/arm/armopts.s.in \
    libs/opus/celt/arm/celt_pitch_xcorr_arm.s \

# exclude CELT support if requested
!contains(CONFIG, "nocelt") {
    message(Legacy support for CELT enabled.)

    INCLUDEPATH += $$INCLUDEPATH_CELT
    HEADERS += $$HEADERS_CELT
    SOURCES += $$SOURCES_CELT
    DISTFILES += $$DISTFILES_CELT
    DEFINES += USE_LEGACY_CELT
}

# use external OPUS library if requested
contains(CONFIG, "opus_shared_lib") {
    message(OPUS codec is used from a shared library.)

    unix {
        !exists(/usr/include/opus/opus_custom.h) {
            !exists(/usr/local/include/opus/opus_custom.h) {
                 message(Header opus_custom.h was not found at the usual place. Maybe the opus dev packet is missing.)
            }
        }

        LIBS += -lopus
        DEFINES += USE_OPUS_SHARED_LIB
    }
} else {
    INCLUDEPATH += $$INCLUDEPATH_OPUS
    HEADERS += $$HEADERS_OPUS
    SOURCES += $$SOURCES_OPUS
    DISTFILES += $$DISTFILES_OPUS
}
