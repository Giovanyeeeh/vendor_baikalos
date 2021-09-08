/*
 * Copyright (c) 2019-2020 The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define LOG_TAG "android.hardware.power.baikalos"

#include "Power.h"

#include <android-base/file.h>
#include <android-base/logging.h>

#include <aidl/android/hardware/power/BnPower.h>

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include <cutils/properties.h>

#include "utils.h"


using ::aidl::android::hardware::power::BnPower;
using ::aidl::android::hardware::power::Boost;
using ::aidl::android::hardware::power::IPower;
using ::aidl::android::hardware::power::Mode;

using ::ndk::ScopedAStatus;
using ::ndk::SharedRefBase;

namespace aidl {
namespace android {
namespace hardware {
namespace power {
namespace impl {

#ifdef MODE_EXT
extern bool isDeviceSpecificModeSupported(Power* power, Mode type, bool* _aidl_return);
extern bool setDeviceSpecificMode(Power* power, Mode type, bool enabled);
extern bool isDeviceSpecificBoostSupported(Power* power, int32_t type, bool* _aidl_return);
extern bool setDeviceSpecificBoost(Power* power, int32_t type, int32_t boost);
extern bool setPerformanceProfile(Power* power, int32_t type);
extern bool setThermalProfile(Power* power, int32_t type);
#endif

ndk::ScopedAStatus Power::setMode(Mode type, bool enabled) {
    //LOG(INFO) << "Power setMode: " << static_cast<int32_t>(type) << " to: " << enabled;
#ifdef MODE_EXT
    if (setDeviceSpecificMode(this, type, enabled)) {
        return ndk::ScopedAStatus::ok();
    }
#endif
    switch (type) {
#ifdef TAP_TO_WAKE_NODE
        case Mode::DOUBLE_TAP_TO_WAKE:
            ::android::base::WriteStringToFile(enabled ? "1" : "0", TAP_TO_WAKE_NODE, true);
            break;
#else
        case Mode::DOUBLE_TAP_TO_WAKE:
            break;
#endif
        case Mode::LOW_POWER:
            setBoost(static_cast<Boost>(12101), enabled ? 1 : 0);
            break;
        case Mode::EXPENSIVE_RENDERING:
            setBoost(static_cast<Boost>(12002), enabled ? 1 : 0);
            break;
        case Mode::DEVICE_IDLE:
            setBoost(static_cast<Boost>(12103), enabled ? 1 : 0);
            break;
        case Mode::DISPLAY_INACTIVE:
            setBoost(static_cast<Boost>(12102), enabled ? 1 : 0);
            break;
        case Mode::AUDIO_STREAMING_LOW_LATENCY:
            setBoost(static_cast<Boost>(12003), enabled ? 1 : 0);
            break;

        case Mode::CAMERA_STREAMING_SECURE:
        case Mode::CAMERA_STREAMING_LOW:
        case Mode::CAMERA_STREAMING_MID:
        case Mode::CAMERA_STREAMING_HIGH:
            setBoost(static_cast<Boost>(12004), enabled ? 1 : 0);
            break;

        case Mode::VR:
            setBoost(static_cast<Boost>(12005), enabled ? 1 : 0);
            break;

        case Mode::LAUNCH:
            setBoost(static_cast<Boost>(12001), enabled ? 1 : 0);
            break;

        case Mode::INTERACTIVE:
            //setInteractive(enabled);
            break;
        case Mode::SUSTAINED_PERFORMANCE:
            setBoost(static_cast<Boost>(12004), enabled ? 1 : 0);
            break;

        case Mode::FIXED_PERFORMANCE:
            setBoost(static_cast<Boost>(12003), enabled ? 1 : 0);
            break;

        default:
            LOG(INFO) << "Mode " << static_cast<int32_t>(type) << "Not Supported";
            break;
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Power::isModeSupported(Mode type, bool* _aidl_return) {
    //LOG(INFO) << "Power isModeSupported: " << static_cast<int32_t>(type);

#ifdef MODE_EXT
    if (isDeviceSpecificModeSupported(this, type, _aidl_return)) {
        return ndk::ScopedAStatus::ok();
    }
#endif

    switch (type) {
#ifdef TAP_TO_WAKE_NODE
        case Mode::DOUBLE_TAP_TO_WAKE:
#endif
        case Mode::LAUNCH:
        case Mode::INTERACTIVE:
        case Mode::SUSTAINED_PERFORMANCE:
        case Mode::FIXED_PERFORMANCE:
            *_aidl_return = true;
            break;
        default:
            //*_aidl_return = false;
            *_aidl_return = true;
            break;
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Power::setBoost(Boost type, int32_t durationMs) {

//    bool mInteractionBoost;
//    bool mRenderBoost;
//    bool mCameraBoost;
//    bool mAudioBoost;
//    bool mLaunchBoost;
//    bool mLowPower;
//    bool mDeviceIdle;
//    bool mScreenOff;
//    bool mVRBoost;

    if( isDebug() ) LOG(INFO) << "Power setBoost: " << static_cast<int32_t>(type)
                 << ", duration: " << durationMs;

    switch (static_cast<int>(type)) {
        case static_cast<int>(Boost::INTERACTION):
            //power_hint(POWER_HINT_INTERACTION, &durationMs);
            //break;
            break;

        case 10000: // Set performance profile
            mCurrentPerformanceProfile = durationMs;
            updateSettings();
            if( !isOverridePerformance() ) {
            if( isDebug() ) LOG(INFO) << "Power setPerformanceProfile: " << durationMs;
                setPerformanceProfile(this, durationMs);
            }
            break; 

        case 11000: // Set thermal profile
            mCurrentThermalProfile = durationMs;
            if( !isOverrideThermal() ) {
                if( isDebug() ) LOG(INFO) << "Power setThermalProfile: " << durationMs;
                setThermalProfile(this, durationMs);
            }
            break;

        case 11001: // Override thermal profile
            if( durationMs > 0 ) {
                mOverrideThermal = true;
                mOverrideThermalProfile = durationMs;
                if( isDebug() ) LOG(INFO) << "Power setOverrideThermalProfile: " << durationMs;
                setThermalProfile(this, mOverrideThermalProfile);
            } else {
                mOverrideThermal = false;
                if( isDebug() ) LOG(INFO) << "Power cancelOverrideThermalProfile: " << mCurrentThermalProfile;
                setThermalProfile(this, mCurrentThermalProfile);
            }
            break;

        case 12000: // Framework INTERACTION boost
            if( durationMs == 1 && (mInteractionBoost || mIncallBoost) ) break;
            if( durationMs == 0 && (!mInteractionBoost && !mIncallBoost) ) break;
            if( !isIgnoreBoost() && !mInteractionBoost ) {
                mInteractionBoost = true;
                if( isDebug() ) LOG(INFO) << "Power Interaction Boost: On" ;
                setDeviceSpecificBoost(this, 12000,durationMs); 
            }
            if( !durationMs ) {
                if( isDebug() && mInteractionBoost ) LOG(INFO) << "Power Interaction Boost: Off" ;
                if( isDebug() && mIncallBoost ) LOG(INFO) << "Incoming call Boost: Off" ;
                mInteractionBoost = false;
                mIncallBoost = false;
                setDeviceSpecificBoost(this, 12000,durationMs); 
                updatePerformanceProfile();
            }
            break;

        case 12010: // Incall boost
            if( durationMs == 1 && mIncallBoost ) break;
            if( durationMs == 0 && !mIncallBoost ) break;
            if( !mIncallBoost ) {
                mIncallBoost = true;
                if( isDebug() ) LOG(INFO) << "Incoming call Boost: On" ;
                setDeviceSpecificBoost(this, 12000,durationMs); 
            }
            if( !durationMs ) {
                mIncallBoost = false;
                if( isDebug() ) LOG(INFO) << "Incoming call Boost: Off" ;
                setDeviceSpecificBoost(this, 12000,durationMs); 
                updatePerformanceProfile();
            }
            break;

        case 12001: // LAUNCH boost
            updateSettings();
            if( durationMs == 1 && mLaunchBoost ) break;
            if( durationMs == 0 && !mLaunchBoost ) break;
            if( !isIgnoreBoost() && !mLaunchBoost ) {
                mLaunchBoost = true;
                if( isDebug() ) LOG(INFO) << "Power Lunch Boost: On" ;
                setDeviceSpecificBoost(this, 12001,durationMs); 
            }
            if( !durationMs ) {
                mLaunchBoost = false;
                if( isDebug() ) LOG(INFO) << "Power Lunch Boost: Off" ;
                setDeviceSpecificBoost(this, 12001,durationMs); 
                updatePerformanceProfile();
            }
            break;

        case 12002: // Expensive Rendering boost
            if( durationMs == 1 && mRenderBoost ) break;
            if( durationMs == 0 && !mRenderBoost ) break;
            if( !isIgnoreBoost() && !mRenderBoost ) {
                mRenderBoost = true;
                if( isDebug() ) LOG(INFO) << "Power Expensive Rendering: On" ;
                setDeviceSpecificBoost(this, 12002,durationMs); 
            }
            if( !durationMs ) {
                mRenderBoost = false;
                if( isDebug() ) LOG(INFO) << "Power Expensive Rendering: Off" ;
                setDeviceSpecificBoost(this, 12002,durationMs); 
                updatePerformanceProfile();
            }
            break;

        case 12003: // Audio Boost ( Performance )
            if( durationMs == 1 && mAudioBoost ) break;
            if( durationMs == 0 && !mAudioBoost ) break;
            if( !isIgnoreBoost() && !mAudioBoost ) {
                mAudioBoost = true;
                if( isDebug() ) LOG(INFO) << "Power Audio Boost: On" ;
                setDeviceSpecificBoost(this, 12003,durationMs); 
            }
            if( !durationMs ) {
                mAudioBoost = false;
                if( isDebug() ) LOG(INFO) << "Power Audio Boost: Off" ;
                setDeviceSpecificBoost(this, 12003,durationMs); 
                updatePerformanceProfile();
            }
            break;

        case 12004: // Camera Boost ( Sustained Performance )
            if( durationMs == 1 && mCameraBoost ) break;
            if( durationMs == 0 && !mCameraBoost ) break;
            if( !isIgnoreBoost() && !mCameraBoost ) {
                mCameraBoost = true;
                setDeviceSpecificBoost(this, 12004,durationMs); 
                if( isDebug() ) LOG(INFO) << "Power Camera Boost: On" ;
            }
            if( !durationMs ) {
                mCameraBoost = false;
                setDeviceSpecificBoost(this, 12004,durationMs); 
                updatePerformanceProfile();
                if( isDebug() ) LOG(INFO) << "Power Camera Boost: Off" ;
            }
            break;

        case 12005: // VR Boost ( AR/VR sustained limited performance )
            if( durationMs == 1 && mVRBoost ) break;
            if( durationMs == 0 && !mVRBoost ) break;
            if( !isIgnoreBoost() && !mVRBoost ) {
                mVRBoost = true;
                setDeviceSpecificBoost(this, 12005,durationMs); 
                if( isDebug() ) LOG(INFO) << "Power VR Boost: On" ;
            }
            if( !durationMs ) {
                mVRBoost = false;
                setDeviceSpecificBoost(this, 12005,durationMs); 
                updatePerformanceProfile();
                if( isDebug() ) LOG(INFO) << "Power VR Boost: Off" ;
            }
            break;

        case 12101: // LowPower
            mLowPower = durationMs;
            LOG(INFO) << "LowPower" ;
            if( isIgnoreBoost() ) {
                updatePerformanceProfile();
            }
            break;

        case 12102: // ScreenOff
            mScreenOff = durationMs;
            LOG(INFO) << "ScreenOff" ;
            if( isIgnoreBoost() ) {
                updatePerformanceProfile();
            }
            break;

        case 12103: // DeviceIdle
            mDeviceIdle = durationMs;
            LOG(INFO) << "DeviceIdle" ;
            if( isIgnoreBoost() ) {
                updatePerformanceProfile();
            }
            break;

        default:

#ifdef MODE_EXT
            if (setDeviceSpecificBoost(this, (int)type, durationMs)) {
                return ndk::ScopedAStatus::ok();
            }
#endif

            LOG(INFO) << "Boost " << static_cast<int32_t>(type) << " Not Supported";
            break;
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Power::isBoostSupported(Boost type, bool* _aidl_return) {
    //LOG(INFO) << "Power isBoostSupported: " << static_cast<int32_t>(type);

#ifdef MODE_EXT
    if (isDeviceSpecificBoostSupported(this, static_cast<int32_t>(type), _aidl_return)) {
        return ndk::ScopedAStatus::ok();
    }
#endif

    switch (type) {
        case Boost::INTERACTION:
            *_aidl_return = true;
            break;
        default:
            *_aidl_return = false;
            break;
    }
    return ndk::ScopedAStatus::ok();
}


bool Power::isOverridePerformance() {
    return mInteractionBoost ||
    mIncallBoost ||
    mRenderBoost ||
    mCameraBoost ||
    mAudioBoost ||
    mLaunchBoost ||
    mVRBoost;
}

bool Power::isOverrideThermal() {
    return mOverrideThermal;
}

bool Power::isIgnoreBoost() {
    return mBoostEnabled == 0 || mLowPower || mScreenOff;
    //mDeviceIdle ||
   
}

bool Power::isBoostOverride() {
    return mBoostOverride;    
}

void Power::updatePerformanceProfile() {
    if( /*isIgnoreBoost() ||*/ !isOverridePerformance() ) {
        setPerformanceProfile(this, mCurrentPerformanceProfile);
    } else {
        if( isDebug() ) {
            LOG(INFO) << "Ignore boost:" <<  mBoostEnabled << ":" << mLowPower << ":"  << mScreenOff;
            LOG(INFO) << "Override:" << mInteractionBoost << ":" << mRenderBoost << ":" <<  mCameraBoost << ":" <<  mAudioBoost << ":" <<  mLaunchBoost << ":" << mVRBoost << ":" << mIncallBoost;
        }
    }
}

void Power::updateThermalProfile() {
    if( !isOverrideThermal() ) { 
        setThermalProfile(this, mCurrentThermalProfile);
    }
}

void Power::updateSettings() {
    mBoostOverride = property_get_bool("persist.baikal.boost_ovr", false);
    mBoostEnabled = property_get_int32("persist.baikal.boost_en", 0);
    mDebug = property_get_bool("persist.baikal.boost_debug", false);
    __utils_debug = mDebug;
}

bool Power::isDebug() {
    return mDebug;    
}


}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace android
}  // namespace aidl
