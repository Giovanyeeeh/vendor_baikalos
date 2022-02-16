# Include baikalos versions
include $(TOPDIR)vendor/baikalos/build/core/main_version.mk


ifeq ($(PRODUCT_OVERRIDE_INFO),true)
ADDITIONAL_BUILD_PROPERTIES += \
    ro.build.stock_fingerprint=$(PRODUCT_OVERRIDE_FINGERPRINT) \
    ro.build.stock_sec_patch=$(PRODUCT_OVERRIDE_SECURITY_PATCH)

# Override product info for Google Play Services and SafetyNet
ADDITIONAL_BUILD_PROPERTIES += \
    ro.build.tags=release-keys \
    ro.odm.build.tags=release-keys \
    ro.product.build.tags=release-keys \
    ro.system.build.tags=release-keys \
    ro.system_ext.build.tags=release-keys \
    ro.vendor.build.tags=release-keys \
    ro.build.flavor=$(PRODUCT_NAME)-user \

# Description needs special treatment because it contains spaces
PRIVATE_BUILD_DESC := $(PRODUCT_OVERRIDE_DESC)
endif
