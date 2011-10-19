/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <sys/types.h>

#include <binder/Parcel.h>

#include <utils/Log.h>
#include <utils/String8.h>

#include <bridge/IBridgeService.h>

#undef LOG_TAG
#define LOG_TAG "IBridgeService"

namespace android
{

enum
{
    SEND_COMMAND = IBinder::FIRST_CALL_TRANSACTION
};

class BpBridgeService : public BpInterface<IBridgeService>
{

public:

    BpBridgeService(const sp<IBinder>& impl)
        : BpInterface<IBridgeService>(impl)
    {
    }

    virtual const char* sendCommand(const char *command)
    {
        const char *return_string;
        Parcel data, reply;

        data.writeInterfaceToken(IBridgeService::getInterfaceDescriptor());
        data.writeString16(String16(command));
        remote()->transact(SEND_COMMAND, data, &reply);

        reply.readInt32();
        return_string = String8(reply.readString16()).string();

        LOGD("sendCommand(%s): return_string[%d][%s]", command, strlen(return_string), return_string);

        return return_string;
    }

}; // class BpBridgeService

IMPLEMENT_META_INTERFACE(BridgeService, "com.lge.bridges.IBridgeService");

}; // namespace android
