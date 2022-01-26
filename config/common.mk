# Allow vendor/extra to override any property by setting it first
$(call inherit-product-if-exists, vendor/extra/product.mk)

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0

# Include BAIKALOS version
include vendor/baikalos/config/baikalos_version.mk

# Include baikalos packages
include vendor/baikalos/config/baikalos_packages.mk

# Include baikalos version
include vendor/baikalos/config/baikalos_props.mk

# AOSP recovery flashing
ifeq ($(TARGET_USES_AOSP_RECOVERY),true)
PRODUCT_SYSTEM_DEFAULT_PROPERTIES += \
    persist.sys.recovery_update=true
endif

# Disable extra StrictMode features on all non-engineering builds
PRODUCT_SYSTEM_DEFAULT_PROPERTIES += persist.sys.strictmode.disable=true

# Include AOSP audio files
include vendor/baikalos/config/aosp_audio.mk

# Google sounds
include vendor/baikalos/google/GoogleAudio.mk

# PRODUCT_ENFORCE_RRO_TARGETS := *
# PRODUCT_ENFORCE_RRO_EXCLUDED_OVERLAYS += vendor/baikalos/overlay
PRODUCT_ENFORCE_RRO_EXCLUDED_OVERLAYS += vendor/baikalos/overlay/common
PRODUCT_PACKAGE_OVERLAYS += vendor/baikalos/overlay/common
#PRODUCT_ENFORCE_RRO_TARGETS := Launcher3 Launcher3Go Launcher3QuickStep Launcher3QuickStepGo AicpExtras DocumentsUI Settings ThemePicker


# TWRP
ifeq ($(BUILD_TWRP),true)
RECOVERY_TYPE := twrp
else
RECOVERY_TYPE := aosp
endif

# Copy all baikalos specific init rc files
$(foreach f,$(wildcard vendor/baikalos/prebuilt/common/etc/init/*.rc),\
    $(eval PRODUCT_COPY_FILES += $(f):system/etc/init/$(notdir $f)))

# Do not include art debug targets
PRODUCT_ART_TARGET_INCLUDE_DEBUG_BUILD := false

# Strip the local variable table and the local variable type table to reduce
# the size of the system image. This has no bearing on stack traces, but will
# leave less information available via JDWP.
PRODUCT_MINIMIZE_JAVA_DEBUG_INFO := true

# Disable vendor restrictions
PRODUCT_RESTRICT_VENDOR_FILES := false

# Clean cache script
PRODUCT_COPY_FILES += \
    vendor/baikalos/prebuilt/common/bin/clean_cache.sh:$(TARGET_COPY_OUT_SYSTEM)/bin/clean_cache.sh

# Backup Tool
PRODUCT_COPY_FILES += \
    vendor/baikalos/prebuilt/common/bin/backuptool.sh:install/bin/backuptool.sh \
    vendor/baikalos/prebuilt/common/bin/backuptool.functions:install/bin/backuptool.functions \
    vendor/baikalos/prebuilt/common/bin/50-baikalos.sh:$(TARGET_COPY_OUT_SYSTEM)/addon.d/50-baikalos.sh


# swap scripts
PRODUCT_COPY_FILES += \
    vendor/baikalos/prebuilt/common/bin/init.swap.zram_on.sh:$(TARGET_COPY_OUT_VENDOR)/bin/init.swap.zram_on.sh \
    vendor/baikalos/prebuilt/common/bin/init.swap.zram_off.sh:$(TARGET_COPY_OUT_VENDOR)/bin/init.swap.zram_off.sh \
    vendor/baikalos/prebuilt/common/bin/init.swap.disk_on.sh:$(TARGET_COPY_OUT_VENDOR)/bin/init.swap.disk_on.sh \
    vendor/baikalos/prebuilt/common/bin/init.swap.disk_off.sh:$(TARGET_COPY_OUT_VENDOR)/bin/init.swap.disk_off.sh \



ifneq ($(strip $(AB_OTA_PARTITIONS) $(AB_OTA_POSTINSTALL_CONFIG)),)
PRODUCT_COPY_FILES += \
    vendor/baikalos/prebuilt/common/bin/backuptool_ab.sh:$(TARGET_COPY_OUT_SYSTEM)/bin/backuptool_ab.sh \
    vendor/baikalos/prebuilt/common/bin/backuptool_ab.functions:$(TARGET_COPY_OUT_SYSTEM)/bin/backuptool_ab.functions \
    vendor/baikalos/prebuilt/common/bin/backuptool_postinstall.sh:$(TARGET_COPY_OUT_SYSTEM)/bin/backuptool_postinstall.sh
ifneq ($(TARGET_BUILD_VARIANT),user)
PRODUCT_SYSTEM_DEFAULT_PROPERTIES += \
    ro.ota.allow_downgrade=true
endif
endif

# system mount
PRODUCT_COPY_FILES += \
    vendor/baikalos/prebuilt/common/bin/system-mount.sh:install/bin/system-mount.sh

# Dex preopt
PRODUCT_DEXPREOPT_SPEED_APPS += \
    SystemUI \
    Launcher3QuickStep


# Pre-granted permissions baikalos-permissions.xml
PRODUCT_COPY_FILES += \
    vendor/baikalos/config/permissions/baikalos-permissions.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/default-permissions/baikalos-permissions.xml

# Backup Services whitelist
PRODUCT_COPY_FILES += \
    vendor/baikalos/config/permissions/backup.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/sysconfig/backup.xml \
    vendor/baikalos/config/permissions/privapp-permissions-livedisplay.xml:$(TARGET_COPY_OUT_PRODUCT)/etc/permissions/privapp-permissions-livedisplay.xml

# Power whitelist
PRODUCT_COPY_FILES += \
    vendor/baikalos/config/permissions/baikalos-power-whitelist.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/sysconfig/baikalos-power-whitelist.xml

# baikalos permissions
PRODUCT_COPY_FILES += \
    vendor/baikalos/config/permissions/privapp-permissions-baikalos-system.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/permissions/privapp-permissions-baikalos.xml \
    vendor/baikalos/config/permissions/privapp-permissions-baikalos-system-ext.xml:$(TARGET_COPY_OUT_SYSTEM_EXT)/etc/permissions/privapp-permissions-baikalos.xml \
    vendor/baikalos/config/permissions/privapp-permissions-baikalos-product.xml:$(TARGET_COPY_OUT_PRODUCT)/etc/permissions/privapp-permissions-baikalos.xml

# baikalos GAPPS permissions
PRODUCT_COPY_FILES += \
    vendor/baikalos/config/permissions/privapp-permissions-baikalos-google.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/permissions/privapp-permissions-baikalos-google.xml \
    vendor/baikalos/config/permissions/privapp-permissions-baikalos-google-product.xml:$(TARGET_COPY_OUT_PRODUCT)/etc/permissions/privapp-permissions-baikalos-google-product.xml

# Hidden API whitelist
PRODUCT_COPY_FILES += \
    vendor/baikalos/config/permissions/baikalos-hiddenapi-package-whitelist.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/permissions/baikalos-hiddenapi-package-whitelist.xml

PRODUCT_COPY_FILES += \
    vendor/baikalos/config/permissions/lineage-hiddenapi-package-whitelist.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/permissions/lineage-hiddenapi-package-whitelist.xml


#$(warning $(call find-copy-files,*,vendor/baikalos/prebuilt/common/fonts,$(TARGET_COPY_OUT_PRODUCT)/fonts))

# Fonts
PRODUCT_COPY_FILES += \
    $(call find-copy-subdir-files,*,vendor/baikalos/prebuilt/common/fonts,$(TARGET_COPY_OUT_PRODUCT)/fonts) \
    vendor/baikalos/prebuilt/common/etc/fonts_customization.xml:$(TARGET_COPY_OUT_PRODUCT)/etc/fonts_customization.xml

# Enable Android Beam on all targets
PRODUCT_COPY_FILES += \
    vendor/baikalos/config/permissions/android.software.nfc.beam.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/permissions/android.software.nfc.beam.xml

# Enable SIP+VoIP on all targets
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.software.sip.voip.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/permissions/android.software.sip.voip.xml

# Copy over added mimetype supported in libcore.net.MimeUtils
PRODUCT_COPY_FILES += \
    vendor/baikalos/prebuilt/common/lib/content-types.properties:$(TARGET_COPY_OUT_SYSTEM)/lib/content-types.properties

# Google extra permissions and features
PRODUCT_COPY_FILES += \
    vendor/baikalos/config/permissions/com.google.android.dialer.support.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/permissions/com.google.android.dialer.support.xml \
    vendor/baikalos/config/permissions/com.google.android.feature.ANDROID_ONE_EXPERIENCE.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/permissions/com.google.android.feature.ANDROID_ONE_EXPERIENCE.xml \
    vendor/baikalos/config/permissions/privapp-permissions-hotword.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/permissions/privapp-permissions-hotword.xml \
    vendor/baikalos/config/permissions/google_build.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/sysconfig/google_build.xml \
    vendor/baikalos/config/permissions/nexus.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/sysconfig/nexus.xml \
    vendor/baikalos/config/permissions/pixel_2016_exclusive.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/sysconfig/pixel_2016_exclusive.xml \
    vendor/baikalos/config/permissions/pixel_2017_exclusive.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/sysconfig/pixel_2017_exclusive.xml \
    vendor/baikalos/config/permissions/pixel_2018_exclusive.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/sysconfig/pixel_2018_exclusive.xml \
    vendor/baikalos/config/permissions/pixel_2019_midyear_exclusive.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/sysconfig/pixel_2019_midyear_exclusive.xml \
    vendor/baikalos/config/permissions/pixel_experience_2017.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/sysconfig/pixel_experience_2017.xml \
    vendor/baikalos/config/permissions/pixel_experience_2018.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/sysconfig/pixel_experience_2018.xml \
    vendor/baikalos/config/permissions/pixel_experience_2019_midyear.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/sysconfig/pixel_experience_2019_midyear.xml

# Google extra libraries (sketch/swipe)
PRODUCT_COPY_FILES += \
    vendor/baikalos/prebuilt/common/lib/libsketchology_native.so:$(TARGET_COPY_OUT_SYSTEM)/lib/libsketchology_native.so \
    vendor/baikalos/prebuilt/common/lib64/libsketchology_native.so:$(TARGET_COPY_OUT_SYSTEM)/lib64/libsketchology_native.so \
    #vendor/baikalos/prebuilt/common/bin/keystore:$(TARGET_COPY_OUT_SYSTEM)/bin/keystore \
    #vendor/baikalos/prebuilt/common/lib64/libkeystore-attestation-application-id.so:$(TARGET_COPY_OUT_SYSTEM)/lib64/libkeystore-attestation-application-id.so


# Disable vendor restrictions
PRODUCT_RESTRICT_VENDOR_FILES := false

# Navbar
PRODUCT_PACKAGES += \
    GesturalNavigationOverlayLong \
    GesturalNavigationOverlayMedium \
    GesturalNavigationOverlayHidden

