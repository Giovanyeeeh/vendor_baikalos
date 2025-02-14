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

#ifndef ANDROID_HARDWARE_POWER_POWER_H
#define ANDROID_HARDWARE_POWER_POWER_H

#include <aidl/android/hardware/power/BnPower.h>

namespace aidl {
namespace android {
namespace hardware {
namespace power {
namespace impl {

class Power : public BnPower {
  public:
    Power() : BnPower() {  }
    ndk::ScopedAStatus setMode(Mode type, bool enabled) override;
    ndk::ScopedAStatus isModeSupported(Mode type, bool* _aidl_return) override;
    ndk::ScopedAStatus setBoost(Boost type, int32_t durationMs) override;
    ndk::ScopedAStatus isBoostSupported(Boost type, bool* _aidl_return) override;

  public:
    bool isOverridePerformance();
    bool isOverrideThermal();
    bool isIgnoreBoost();
    bool isBoostOverride();
    bool isDebug();
    inline int getBoostLevel() { return mBoostEnabled; };
    inline int getHighPerf() { return mHighPerf; };
    

  protected:
    void updatePerformanceProfile();
    void updateThermalProfile();
    void updateSettings();

  protected:
    int32_t mCurrentPerformanceProfile;
    int32_t mCurrentThermalProfile;
    bool mInteractionBoost;
    bool mRenderBoost;
    bool mCameraBoost;
    bool mAudioBoost;
    bool mLaunchBoost;
    bool mLowPower;
    bool mDeviceIdle;
    bool mScreenOff;
    bool mVRBoost;
    bool mIncallBoost;

    bool mOverrideThermal;
    int32_t mOverrideThermalProfile;
    bool mBoostOverride;
    int mBoostEnabled;
    bool mDebug;
    int mHighPerf;
    
};

}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace android
}  // namespace aidl
#endif  // ANDROID_HARDWARE_POWER_POWER_H
