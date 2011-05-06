/******************************************************************************
 * GPS HAL shim
 * load older GPS libraries on gingerbread (and above?)
 *
 * Copyright 2010-2011 - Ricardo Cerqueira, The CyanogenMod Project
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
 *
 ******************************************************************************/

#include <hardware/gps.h>

/** List of older GPS interfaces. */


/* XTRA */
typedef struct {
    gps_xtra_download_request download_request_cb;
} OldGpsXtraCallbacks;

typedef struct {
    int  (*init)( OldGpsXtraCallbacks* callbacks );
    int  (*inject_xtra_data)( char* data, int length );
} OldGpsXtraInterface;

static int xtraStatus = 0;
/* AGPS */
typedef struct {
    AGpsType        type;
    AGpsStatusValue status;
} OldAGpsStatus;

typedef void (* old_agps_status_callback)(OldAGpsStatus* status);

typedef struct {
    old_agps_status_callback status_cb;
} OldAGpsCallbacks;

/* CONFIRMED */
/*
 * pos12 - setID
 */
typedef struct {
    void  (*init)( OldAGpsCallbacks* callbacks );
    int  (*filler1)();
    int  (*filler2)();
    int  (*filler3)();
    int  (*filler4)();
    int  (*filler5)();
    int  (*filler6)();
    int  (*filler7)();
    int  (*data_conn_open)( const char* apn );
    int  (*data_conn_closed)();
    int  (*data_conn_failed)();
    int  (*set_id)();
    int  (*set_server)( AGpsType type, const char* hostname, int port );
} OldAGpsInterface;

/* AGPS RIL */
typedef struct {
    agps_ril_request_set_id request_setid;
    agps_ril_request_ref_loc request_refloc;
} OldAGpsRilCallbacks;

typedef struct {
    void  (*init)( OldAGpsRilCallbacks* callbacks );
    void (*set_ref_location) (const AGpsRefLocation *agps_reflocation, size_t sz_struct);
    void (*set_set_id) (AGpsSetIDType type, const char* setid);
    void (*ni_message) (uint8_t *msg, size_t len);
} OldAGpsRilInterface;

/* NI */
typedef struct
{
    void (*init) (GpsNiCallbacks *callbacks);
    void (*respond) (int notif_id, GpsUserResponseType user_response);
} OldGpsNiInterface;

/* Core structures */
typedef struct {
    uint16_t        flags;
    double          latitude;
    double          longitude;
    double          altitude;
    float           speed;
    float           bearing;
    float           accuracy;
    GpsUtcTime      timestamp;
} OldGpsLocation;

typedef struct {
    GpsStatusValue status;
} OldGpsStatus;

typedef struct {
    int     prn;
    float   snr;
    float   elevation;
    float   azimuth;
} OldGpsSvInfo;

typedef struct {
    int         num_svs;
    OldGpsSvInfo   sv_list[GPS_MAX_SVS];
    uint32_t    ephemeris_mask;
    uint32_t    almanac_mask;
    uint32_t    used_in_fix_mask;
} OldGpsSvStatus;


/* Core GPS HAL Callbacks */
typedef void (* old_gps_location_callback)(OldGpsLocation* location);
typedef void (* old_gps_status_callback)(OldGpsStatus* status);
typedef void (* old_gps_sv_status_callback)(OldGpsSvStatus* sv_info);

typedef struct {
    old_gps_location_callback location_cb;
    old_gps_status_callback status_cb;
    old_gps_sv_status_callback sv_status_cb;
    gps_nmea_callback nmea_cb;
} OldGpsCallbacks;


typedef struct {
    int   (*init)( OldGpsCallbacks* callbacks );
    int   (*start)( void );
    int   (*stop)( void );
    void  (*cleanup)( void );
    int   (*inject_time)(GpsUtcTime time, int64_t timeReference,
            int uncertainty);
    int  (*inject_location)(double latitude, double longitude, float accuracy);
    void  (*delete_aiding_data)(GpsAidingData flags);
    int   (*set_position_mode)(GpsPositionMode mode, int fix_frequency);
    const void* (*get_extension)(const char* name);
} OldGpsInterface;


