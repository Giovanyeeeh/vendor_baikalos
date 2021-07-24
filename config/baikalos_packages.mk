# Required packages
PRODUCT_PACKAGES += \
    CellBroadcastReceiver \
    SpareParts \
#    LockClock

# Optional packages
PRODUCT_PACKAGES += \
    Basic \
    PhaseBeam \
    ThemePicker \
    WallpaperPicker2

# su
PRODUCT_PACKAGES += \
    adb_root
#ifeq ($(WITH_SU),true)
#PRODUCT_PACKAGES += \
#    su
#endif

# Extra Optional packages
PRODUCT_PACKAGES += \
    BluetoothExt \
    Email \
    Etar \
    ExactCalculator \
    Exchange2
#    Launcher3Dark

# MusicFX
PRODUCT_PACKAGES += \
    MusicFX

# Phonograph
PRODUCT_PACKAGES += \
    Phonograph

# Jelly
PRODUCT_PACKAGES += \
    Jelly

# OnDeviceAppPrediction
PRODUCT_PACKAGES += \
    AppPredictionService

# OmniJaws
PRODUCT_PACKAGES += \
    OmniJaws

# OmniStyle
PRODUCT_PACKAGES += \
    OmniStyle

# SystemUI plugins
PRODUCT_PACKAGES += \
    QuickAccessWallet

# Config
PRODUCT_PACKAGES += \
    SimpleDeviceConfig

# Fonts packages
#PRODUCT_PACKAGES += \
#    invictrix-fonts

# Extra tools
PRODUCT_PACKAGES += \
    openvpn \
    e2fsck \
    mke2fs \
    tune2fs \
    fsck.exfat \
    mkfs.exfat \
    ntfsfix \
    ntfs-3g \
    fsck.ntfs \
    mkfs.ntfs \
    mount.ntfs

PRODUCT_PACKAGES += \
    charger_res_images

# Custom off-mode charger
ifeq ($(WITH_BAIKALOS_CHARGER),true)
PRODUCT_PACKAGES += \
    baikalos_charger_res_images \
    font_log.png \
    libhealthd.baikalos
endif

# Extra tools
PRODUCT_PACKAGES += \
    7z \
    awk \
    bash \
    bzip2 \
    curl \
    getcap \
    htop \
    lib7z \
    libsepol \
    nano \
    pigz \
    powertop \
    setcap \
    unrar \
    unzip \
    vim \
    wget \
    zip

#PRODUCT_PACKAGES += \
#    AndroidDarkThemeOverlay \
#    SettingsDarkThemeOverlay

# Boot animation include
ifneq ($(TARGET_SCREEN_WIDTH) $(TARGET_SCREEN_HEIGHT),$(space))

# determine the smaller dimension
TARGET_BOOTANIMATION_SIZE := $(shell \
  if [ $(TARGET_SCREEN_WIDTH) -lt $(TARGET_SCREEN_HEIGHT) ]; then \
    echo $(TARGET_SCREEN_WIDTH); \
  else \
    echo $(TARGET_SCREEN_HEIGHT); \
  fi )

# get a sorted list of the sizes
bootanimation_sizes := $(subst .zip,, $(shell ls vendor/baikalos/prebuilt/common/bootanimation))
bootanimation_sizes := $(shell echo -e $(subst $(space),'\n',$(bootanimation_sizes)) | sort -rn)

# find the appropriate size and set
define check_and_set_bootanimation
$(eval TARGET_BOOTANIMATION_NAME := $(shell \
  if [ -z "$(TARGET_BOOTANIMATION_NAME)" ]; then \
    if [ $(1) -le $(TARGET_BOOTANIMATION_SIZE) ]; then \
      echo $(1); \
      exit 0; \
    fi; \
  fi; \
  echo $(TARGET_BOOTANIMATION_NAME); ))
endef
$(foreach size,$(bootanimation_sizes), $(call check_and_set_bootanimation,$(size)))

ifeq ($(TARGET_BOOTANIMATION_HALF_RES),true)
PRODUCT_COPY_FILES += \
    vendor/baikalos/prebuilt/common/bootanimation/halfres/$(TARGET_BOOTANIMATION_NAME).zip:system/media/bootanimation.zip
else
PRODUCT_COPY_FILES += \
    vendor/baikalos/prebuilt/common/bootanimation/$(TARGET_BOOTANIMATION_NAME).zip:system/media/bootanimation.zip
endif
endif

# baikalos OTA
ifneq ($(BAIKALOS_BUILD_TYPE),DEV)
    PRODUCT_PACKAGES +=  \
       BaikalDelta
endif

# baikalos packages
PRODUCT_PACKAGES += \
    AicpExtras \
    BaikalExtras \
    AicpThemesStub \
    FontAclonicaSourceOverlay \
    FontAmaranteSourceOverlay \
    FontArbutusSourceOverlay \
    FontArvoLatoOverlay \
    FontBariolSourceOverlay \
    FontCagliostroSourceOverlay \
    FontCaviarDreamsOverlay \
    FontCircularStdOverlay \
    FontComfortaaOverlay \
    FontComicSansSourceOverlay \
    FontCoolstorySourceOverlay \
    FontExotwoOverlay \
    FontFiraSansSourceOverlay \
    FontGoogleSansOverlay \
    FontInterOverlay \
    FontLGSmartGothicSourceOverlay \
    FontLinotteOverlay \
    FontOpenSansOverlay \
    FontOxaniumOverlay \
    FontRosemarySourceOverlay \
    FontRubikRubikOverlay \
    FontSamsungOneSourceOverlay \
    FontSFProOverlay \
    FontSignikaOverlay \
    FontSlateForOnePlusOverlay \
    FontSnigletOverlay \
    FontSonySketchSourceOverlay \
    FontSurferSourceOverlay \
    FontUbuntuOverlay \
    FontUbuntuOverlay \
    FontMilanPro11Overlay \
    FontMilanPro12Overlay \
    TilesWallpaper

# baikalos Ad-block
PRODUCT_PACKAGES += \
    hosts.aicp_adblock

# baikalos su
PRODUCT_PACKAGES += \
    baikal_su

#-include packages/overlays/AICP/product_packages.mk
-include packages/overlays/BaikalOS/product_packages.mk


# Face Unlock
TARGET_FACE_UNLOCK_SUPPORTED := false
ifneq ($(TARGET_DISABLE_ALTERNATIVE_FACE_UNLOCK), true)
PRODUCT_PACKAGES += \
    FaceUnlockService
TARGET_FACE_UNLOCK_SUPPORTED := true
endif
PRODUCT_SYSTEM_DEFAULT_PROPERTIES += \
    ro.face.moto_unlock_service=$(TARGET_FACE_UNLOCK_SUPPORTED)

#ifeq ($(TARGET_USE_MOTO_DOLBY), true)
#    $(warning TARGET_USE_MOTO_DOLBY=true)
#    $(call inherit-product, external/motorola/dolby/dolby.mk)
#    PRODUCT_PACKAGES += \
#        daxService \
#        MotoDolbyV3
#else
#    $(warning TARGET_USE_MOTO_DOLBY=false)
#endif

LOCAL_OVERRIDES_PACKAGES += com.android.messaging_whitelist.xml
