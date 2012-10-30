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
#include <sys/resource.h>
#include <sys/prctl.h>
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
static short stop_pending;

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

    /* Don't let anything stall the cancellation */
    setpriority(PRIO_PROCESS, 0, -5);

    /* Name the thread to help identify lost ones... */
    char thread_name[64] = "cmTspDrvStop";
    prctl(PR_SET_NAME, (unsigned long) &thread_name, 0, 0, 0);

    usleep((int)timer*1000);
    if (stop_pending) {
        fd = open("/dev/tspdrv",O_RDWR);
        ioctl(fd,TSPDRV_DISABLE_AMP,&dummy);
        close(fd);
    }
    return 0;
}

static void enable_stop_thread(int timeout_ms) {
    pthread_create( &vibstop_pt, NULL, stopvib, (void *)timeout_ms);
    stop_pending = 1;
}

static void disable_stop_thread() {
    /* This may create dangling threads, but since bionic has no
       pthread_cancel(), it's the best I can come up with */
    int dummy = 0;
    if (stop_pending && !pthread_kill( vibstop_pt, 0 ))
        pthread_detach( vibstop_pt );
    stop_pending = 0;
    ioctl(tspfd,TSPDRV_DISABLE_AMP,&dummy);
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
        disable_stop_thread();
        ioctl(tspfd,TSPDRV_ENABLE_AMP,&actuator);
        ioctl(tspfd,TSPDRV_MAGIC_NUMBER,&actuator);
        write(tspfd,&vibsample,4); // First hit triggers the sample processing
        write(tspfd,&vibsample,4); // Now do it for real
        enable_stop_thread(timeout_ms);
    } else {
        disable_stop_thread();
    }

    return 0;
}

