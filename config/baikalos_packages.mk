# Required packages
PRODUCT_PACKAGES += \
    CellBroadcastReceiver \
    Development \
    SpareParts \
    LockClock 

# Optional packages
PRODUCT_PACKAGES += \
    Basic \
    LiveWallpapersPicker \
    PhaseBeam 

# su
ifeq ($(WITH_SU),true)
PRODUCT_PACKAGES += \
    su
endif

# Extra Optional packages
PRODUCT_PACKAGES += \
    Calculator \
    BluetoothExt \
    Launcher3Dark

# MusicFX
PRODUCT_PACKAGES += \
    MusicFX

# Omni-Record
PRODUCT_PACKAGES += \
    OmniRecord
# Phonograph
PRODUCT_PACKAGES += \
    Phonograph

# Jelly
PRODUCT_PACKAGES += \
    Jelly

# OmniJaws
PRODUCT_PACKAGES += \
    OmniJaws

# Fonts packages
PRODUCT_PACKAGES += \
    invictrix-fonts

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

PRODUCT_PACKAGES += \
    AndroidDarkThemeOverlay \
    SettingsDarkThemeOverlay

# Bootanimation include
PRODUCT_PACKAGES += \
    bootanimation.zip 

# BAIKALOS OTA
ifneq ($(BAIKALOS_BUILDTYPE),UNOFFICIAL)
PRODUCT_PACKAGES +=  \
   Updater
endif

# BAIKALOS packages
PRODUCT_PACKAGES += \
    AicpExtras \
    BaikalOSExtras \
    TilesWallpaper

# include AICP overalys (Themes support)
include packages/overlays/AICP/product_packages.mk

# include BaikalOS overalys
include packages/overlays/baikalos/product_packages.mk

