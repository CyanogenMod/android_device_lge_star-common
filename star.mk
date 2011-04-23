# The gps config appropriate for this device
$(call inherit-product, device/common/gps/gps_us_supl.mk)

DEVICE_PACKAGE_OVERLAYS += device/lge/star-common/overlay

# Board-specific init
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/ueventd.tegra.rc:root/ueventd.tegra.rc

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/recovery/postrecoveryboot.sh:recovery/root/sbin/postrecoveryboot.sh \
    $(LOCAL_PATH)/media_profiles.xml:system/etc/media_profiles.xml \
    $(LOCAL_PATH)/egl.cfg:system/lib/egl/egl.cfg \
    $(LOCAL_PATH)/init.vsnet:system/bin/init.vsnet \
    $(LOCAL_PATH)/prebuilt/rild:system/bin/rild \
    $(LOCAL_PATH)/prebuilt/setup-recovery:system/bin/setup-recovery \
    $(LOCAL_PATH)/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf \
    $(LOCAL_PATH)/dhcpcd.conf:system/etc/dhcpcd/dhcpcd.conf \
    $(LOCAL_PATH)/gps_brcm_conf.xml:system/etc/gps_brcm_conf.xml

## This is a patched version of libicuuc to include the old symbols
#  needed by glgps
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/prebuilt/libicuuc.so:system/lib/libicuuc.so

# Permission files
PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/base/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/base/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/base/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/base/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/base/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/base/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/base/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/base/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/base/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/base/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    frameworks/base/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml

$(call inherit-product, build/target/product/full_base.mk)

PRODUCT_LOCALES += hdpi

## LGE stuffs
PRODUCT_PACKAGES += \
    LGEServices \
    libbridge \
    libbridge_jni \
    libsecureclock \

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0
