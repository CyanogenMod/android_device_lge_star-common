ifeq ($(TARGET_ARCH),arm)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	bridgeutil.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include/

LOCAL_SHARED_LIBRARIES := libcutils libutils libc libbridges

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE:= bridgeutil

include $(BUILD_EXECUTABLE)

endif
