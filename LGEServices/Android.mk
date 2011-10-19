ifneq ($(filter p990 p999, $(TARGET_BOOTLOADER_BOARD_NAME)),)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-java-files-under, java)

LOCAL_SRC_FILES += \
           java/com/lge/bridges/IBridgeService.aidl \
           java/com/lge/secureclock/ISecureClockService.aidl

LOCAL_PACKAGE_NAME := LGEServices 
LOCAL_CERTIFICATE := platform

include $(BUILD_PACKAGE)

include $(call all-makefiles-under,$(LOCAL_PATH))

endif # p990 filter
