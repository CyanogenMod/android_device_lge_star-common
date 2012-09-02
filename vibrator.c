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
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define TSPDRV_MAGIC_NUMBER                 0x494D4D52

#define TSPDRV_STOP_KERNEL_TIMER            _IO(TSPDRV_MAGIC_NUMBER & 0xFF, 1)
#define TSPDRV_ENABLE_AMP                   _IO(TSPDRV_MAGIC_NUMBER & 0xFF, 3)
#define TSPDRV_DISABLE_AMP                  _IO(TSPDRV_MAGIC_NUMBER & 0xFF, 4)

static pthread_t vibstop_pt;
static int tspfd = -1;

int vibrator_exists()
{
    int fd;

    fd = open("/dev/tspdrv", O_RDWR);
    if(fd < 0)
        return 0;
    close(fd);
    return 1;
}

static void* stopvib( void * timer ) {
    int fd;
    int dummy = 0;

    fd = open("/dev/tspdrv",O_RDWR);
    usleep((int)timer*1000);
    ioctl(fd,TSPDRV_DISABLE_AMP,&dummy);
    close(fd);
    return 0;
}

int sendit(int timeout_ms)
{
    int actuator = 0;
    int res = 0;
    int s = 0;
    char vibsample[4];

    vibsample[0] = 0; vibsample[1] = 8; vibsample[2]=1; 
    vibsample[3] = 96;

    if (tspfd < 0)
        tspfd = open("/dev/tspdrv",O_RDWR);

    if (timeout_ms) {
        ioctl(tspfd,TSPDRV_DISABLE_AMP,&actuator);
        pthread_join( vibstop_pt, NULL );
        ioctl(tspfd,TSPDRV_ENABLE_AMP,&actuator);
        ioctl(tspfd,TSPDRV_MAGIC_NUMBER,&actuator);
        write(tspfd,&vibsample,4); // First hit triggers the sample processing
        write(tspfd,&vibsample,4); // Now do it for real
        pthread_create( &vibstop_pt, NULL, stopvib, (void *)timeout_ms);
    } else {
        ioctl(tspfd,TSPDRV_DISABLE_AMP,&actuator);
        pthread_join( vibstop_pt, NULL );
    }

    return 0;
}
