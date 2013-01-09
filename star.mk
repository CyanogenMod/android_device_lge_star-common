DEVICE_PACKAGE_OVERLAYS += device/lge/star-common/overlay

# Board-specific init
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/init_recovery.rc:root/init_recovery.rc \
    $(LOCAL_PATH)/init.cm-star.rc:root/init.cm-star.rc \
    $(LOCAL_PATH)/init.star.usb.rc:root/init.star.usb.rc

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/recovery/postrecoveryboot.sh:recovery/root/sbin/postrecoveryboot.sh \
    $(LOCAL_PATH)/media_profiles.xml:system/etc/media_profiles.xml \
    $(LOCAL_PATH)/media_codecs.xml:system/etc/media_codecs.xml \
    $(LOCAL_PATH)/asound.conf:system/etc/asound.conf \
    $(LOCAL_PATH)/egl.cfg:system/lib/egl/egl.cfg \
    $(LOCAL_PATH)/prebuilt/setup-recovery:system/bin/setup-recovery \
    $(LOCAL_PATH)/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf \
    $(LOCAL_PATH)/dhcpcd.conf:system/etc/dhcpcd/dhcpcd.conf

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/prebuilt/star_synaptics.idc:system/usr/idc/star_synaptics.idc \
    $(LOCAL_PATH)/prebuilt/star_synaptics.kl:system/usr/keylayout/star_synaptics.kl \
    $(LOCAL_PATH)/prebuilt/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
    $(LOCAL_PATH)/prebuilt/tegra-snd-wm8994.kl:system/usr/keylayout/tegra-snd-wm8994.kl \
    $(LOCAL_PATH)/prebuilt/usb_keyboard_102_en_us.kl:system/usr/keylayout/usb_keyboard_102_en_us.kl \
    $(LOCAL_PATH)/prebuilt/usb_keyboard_102_en_us.kcm.bin:system/usr/keychars/usb_keyboard_102_en_us.kcm.bin

# Permission files
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/native/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml


PRODUCT_PROPERTY_OVERRIDES += \
        ro.opengles.version=131072 \
        ro.telephony.call_ring.multiple=false \
        ro.telephony.call_ring.delay=3000 \
        ro.telephony.call_ring.absent=true \
        ro.hardware.respect_als=true \
        ro.bt.bdaddr_path=/sys/devices/platform/bd_address/bdaddr_if \
	debug.sf.electron_frames=42 \
	nv-camera-disable-early-graph=1 \
	dalvik.vm.dexopt-data-only=1 \
	sys.mem.max_hidden_apps=4 \
	ro.lge.audio_soundexception=true \
	persist.service.zram=18

$(call inherit-product, frameworks/native/build/phone-hdpi-512-dalvik-heap.mk)

$(call inherit-product, build/target/product/full_base_telephony.mk)

PRODUCT_LOCALES += hdpi

## Ugly space-saving hack
PRODUCT_PACKAGES += \
    srec-en
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/prebuilt/10-movestuff.sh:system/addon.d/10-movestuff.sh



## LGE stuffs
PRODUCT_PACKAGES += \
    bridgeutil \
    lights.star \
    libbridge \
    libbridge_jni \
    screencap \
    audio.a2dp.default \
    hwcomposer.default \
    com.android.future.usb.accessory

# Enable Torch
PRODUCT_PACKAGES += Torch

# Set default USB interface
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
	persist.sys.usb.config=mtp,adb

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0
PRODUCT_MANUFACTURER := LGE
