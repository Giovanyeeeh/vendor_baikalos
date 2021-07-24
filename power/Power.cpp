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
extern bool isDeviceSpecificModeSupported(Mode type, bool* _aidl_return);
extern bool setDeviceSpecificMode(Mode type, bool enabled);
extern bool isDeviceSpecificBoostSupported(int32_t type, bool* _aidl_return);
extern bool setDeviceSpecificBoost(int32_t type, int32_t boost);
extern bool setPerformanceProfile(int32_t type);
extern bool setThermalProfile(int32_t type);
#endif

ndk::ScopedAStatus Power::setMode(Mode type, bool enabled) {
    LOG(INFO) << "Power setMode: " << static_cast<int32_t>(type) << " to: " << enabled;
#ifdef MODE_EXT
    if (setDeviceSpecificMode(type, enabled)) {
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
    LOG(INFO) << "Power isModeSupported: " << static_cast<int32_t>(type);

#ifdef MODE_EXT
    if (isDeviceSpecificModeSupported(type, _aidl_return)) {
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

    //LOG(INFO) << "Power setBoost: " << static_cast<int32_t>(type)
    //             << ", duration: " << durationMs;

    switch (static_cast<int>(type)) {
        case static_cast<int>(Boost::INTERACTION):
            //power_hint(POWER_HINT_INTERACTION, &durationMs);
            //break;
            break;

        case 10000: // Set performance profile
            mCurrentPerformanceProfile = durationMs;
            if( !isOverridePerformance() ) {
                LOG(INFO) << "Power setPerformanceProfile: " << durationMs;
                setPerformanceProfile(durationMs);
            }
            break; 

        case 11000: // Set thermal profile
            mCurrentThermalProfile = durationMs;
            if( !isOverrideThermal() ) {
                LOG(INFO) << "Power setThermalProfile: " << durationMs;
                setThermalProfile(durationMs);
            }
            break;

        case 11001: // Override thermal profile
            if( durationMs > 0 ) {
                mOverrideThermal = true;
                mOverrideThermalProfile = durationMs;
                LOG(INFO) << "Power setOverrideThermalProfile: " << durationMs;
                setThermalProfile(mOverrideThermalProfile);
            } else {
                mOverrideThermal = false;
                LOG(INFO) << "Power cancelOverrideThermalProfile: " << mCurrentThermalProfile;
                setThermalProfile(mCurrentThermalProfile);
            }
            break;

        case 12000: // Framework INTERACTION boost
            if( durationMs == 1 && mInteractionBoost ) break;
            if( durationMs == 0 && !mInteractionBoost ) break;
            if( !isIgnoreBoost() && !mInteractionBoost ) {
                mInteractionBoost = true;
                LOG(INFO) << "Power Interaction Boost: On" ;
                setDeviceSpecificBoost(12000,durationMs); 
            }
            if( !durationMs ) {
                mInteractionBoost = false;
                LOG(INFO) << "Power Interaction Boost: Off" ;
                setDeviceSpecificBoost(12000,durationMs); 
                updatePerformanceProfile();
            }
            break;

        case 12001: // LAUNCH boost
            if( durationMs == 1 && mLaunchBoost ) break;
            if( durationMs == 0 && !mLaunchBoost ) break;
            if( !isIgnoreBoost() && !mLaunchBoost ) {
                mLaunchBoost = true;
                LOG(INFO) << "Power Lunch Boost: On" ;
                setDeviceSpecificBoost(12001,durationMs); 
            }
            if( !durationMs ) {
                mLaunchBoost = false;
                LOG(INFO) << "Power Lunch Boost: Off" ;
                setDeviceSpecificBoost(12001,durationMs); 
                updatePerformanceProfile();
            }
            break;

        case 12002: // Expensive Rendering boost
            if( durationMs == 1 && mRenderBoost ) break;
            if( durationMs == 0 && !mRenderBoost ) break;
            if( !isIgnoreBoost() && !mRenderBoost ) {
                mRenderBoost = true;
                LOG(INFO) << "Power Expensive Rendering: On" ;
                setDeviceSpecificBoost(12002,durationMs); 
            }
            if( !durationMs ) {
                mRenderBoost = false;
                LOG(INFO) << "Power Expensive Rendering: Off" ;
                setDeviceSpecificBoost(12002,durationMs); 
                updatePerformanceProfile();
            }
            break;

        case 12003: // Audio Boost ( Performance )
            if( durationMs == 1 && mAudioBoost ) break;
            if( durationMs == 0 && !mAudioBoost ) break;
            if( !isIgnoreBoost() && !mAudioBoost ) {
                mAudioBoost = true;
                LOG(INFO) << "Power Audio Boost: On" ;
                setDeviceSpecificBoost(12003,durationMs); 
            }
            if( !durationMs ) {
                mAudioBoost = false;
                LOG(INFO) << "Power Audio Boost: Off" ;
                setDeviceSpecificBoost(12003,durationMs); 
                updatePerformanceProfile();
            }
            break;

        case 12004: // Camera Boost ( Sustained Performance )
            if( durationMs == 1 && mCameraBoost ) break;
            if( durationMs == 0 && !mCameraBoost ) break;
            if( !isIgnoreBoost() && !mCameraBoost ) {
                mCameraBoost = true;
                LOG(INFO) << "Power Camera Boost: On" ;
                setDeviceSpecificBoost(12004,durationMs); 
            }
            if( !durationMs ) {
                mCameraBoost = false;
                LOG(INFO) << "Power Camera Boost: Off" ;
                setDeviceSpecificBoost(12004,durationMs); 
                updatePerformanceProfile();
            }
            break;

        case 12005: // VR Boost ( AR/VR sustained limited performance )
            if( durationMs == 1 && mVRBoost ) break;
            if( durationMs == 0 && !mVRBoost ) break;
            if( !isIgnoreBoost() && !mVRBoost ) {
                mVRBoost = true;
                LOG(INFO) << "Power VR Boost: On" ;
                setDeviceSpecificBoost(12005,durationMs); 
            }
            if( !durationMs ) {
                mVRBoost = false;
                LOG(INFO) << "Power VR Boost: Off" ;
                setDeviceSpecificBoost(12005,durationMs); 
                updatePerformanceProfile();
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
            if (setDeviceSpecificBoost((int)type, durationMs)) {
                return ndk::ScopedAStatus::ok();
            }
#endif

            LOG(INFO) << "Boost " << static_cast<int32_t>(type) << " Not Supported";
            break;
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Power::isBoostSupported(Boost type, bool* _aidl_return) {
    LOG(INFO) << "Power isBoostSupported: " << static_cast<int32_t>(type);

#ifdef MODE_EXT
    if (isDeviceSpecificBoostSupported(static_cast<int32_t>(type), _aidl_return)) {
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
    return mLowPower ||
    mDeviceIdle ||
    mScreenOff;
}

void Power::updatePerformanceProfile() {
    if( isIgnoreBoost() || !isOverridePerformance() ) {
        setPerformanceProfile(mCurrentPerformanceProfile);
    }
}

void Power::updateThermalProfile() {
    if( !isOverrideThermal() ) { 
        setThermalProfile(mCurrentThermalProfile);
    }
}

}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace android
}  // namespace aidl
