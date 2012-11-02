/*
 * Copyright (C) 2012 The CyanogenMod Project
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

#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define TSPDRV_MAGIC_NUMBER                 0x494D4D52

#define TSPDRV_STOP_KERNEL_TIMER            _IO(TSPDRV_MAGIC_NUMBER & 0xFF, 1)
#define TSPDRV_ENABLE_AMP                   _IO(TSPDRV_MAGIC_NUMBER & 0xFF, 3)
#define TSPDRV_DISABLE_AMP                  _IO(TSPDRV_MAGIC_NUMBER & 0xFF, 4)
#define TSPDRV_ENABLE_TIMED_AMP             _IO(TSPDRV_MAGIC_NUMBER & 0xFF, 6)

int vibrator_exists()
{
    int fd;

    fd = open("/dev/tspdrv", O_RDWR);
    if(fd < 0)
        return 0;
    close(fd);
    return 1;
}

int sendit(int timeout_ms)
{
    int fd = open("/dev/tspdrv",O_RDWR);
    int timer = timeout_ms;
    int res = 0;

    if (timeout_ms <= 0) {
        res = ioctl(fd,TSPDRV_DISABLE_AMP,NULL);
    } else {
        res = ioctl(fd,TSPDRV_ENABLE_TIMED_AMP,&timer);
    }

    close(fd);
    return ( res ? -1 : 0 );
}

