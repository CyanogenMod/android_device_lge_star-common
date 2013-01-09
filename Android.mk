ifneq ($(filter p990 p999, $(TARGET_BOOTLOADER_BOARD_NAME)),)
include $(call first-makefiles-under,$(call my-dir))

## Do not let gapps packages install this. It uses a lot of room,
## move it to data and symlink it
include $(CLEAR_VARS)
LOCAL_MODULE := srec-en
LOCAL_MODULE_CLASS := FAKE
LOCAL_MODULE_TAGS := optional
include $(BUILD_SYSTEM)/base_rules.mk
$(LOCAL_BUILT_MODULE): TARGET := /cache/srec/
$(LOCAL_BUILT_MODULE): SYMLINK := $(TARGET_OUT)/usr/srec/en-US
$(LOCAL_BUILT_MODULE):
	$(hide) echo "Symlink: $(SYMLINK) -> $(TARGET)"
	$(hide) mkdir -p $(dir $@)
	$(hide) mkdir -p $(dir $(SYMLINK))
	$(hide) rm -rf $@
	$(hide) rm -rf $(SYMLINK)
	$(hide) ln -sf $(TARGET) $(SYMLINK)
	$(hide) touch $@

endif
