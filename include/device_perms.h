#define CONTROL_PERMS

struct {
    const char *service;
    unsigned int uid;
    unsigned int gid;
} control_perms[] = {
    { "dumpstate",AID_SHELL, AID_LOG },
    { "ril-daemon",AID_RADIO, AID_RADIO },
    { "rawip_vsnet1",AID_RADIO, AID_RADIO },
    { "rawip_vsnet2",AID_RADIO, AID_RADIO },
    { "rawip_vsnet3",AID_RADIO, AID_RADIO },
    { "rawip_vsnet4",AID_RADIO, AID_RADIO },
    { "vsnet1_down", AID_RADIO, AID_RADIO },
    { "vsnet2_down", AID_RADIO, AID_RADIO },
     {NULL, 0, 0 }
};

#define PROPERTY_PERMS

struct {
    const char *prefix;
    unsigned int uid;
    unsigned int gid;
} property_perms[] = {
    { "net.rmnet",        AID_RADIO,    0 },
    { "net.gprs.",        AID_RADIO,    0 },
    { "net.ppp",          AID_RADIO,    0 },
    { "net.qmi",          AID_RADIO,    0 },
    { "net.lte",          AID_RADIO,    0 },
    { "net.cdma",         AID_RADIO,    0 },
    { "ril.",             AID_RADIO,    0 },
    { "gsm.",             AID_RADIO,    0 },
    { "persist.radio",    AID_RADIO,    0 },
    { "net.dns",          AID_RADIO,    0 },
    { "sys.usb.config",   AID_RADIO,    0 },
    { "net.",             AID_SYSTEM,   0 },
    { "dev.",             AID_SYSTEM,   0 },
    { "runtime.",         AID_SYSTEM,   0 },
    { "hw.",              AID_SYSTEM,   0 },
    { "sys.",             AID_SYSTEM,   0 },
    { "service.",         AID_SYSTEM,   0 },
    { "service.",         AID_RADIO,    0 },
    { "wlan.",            AID_SYSTEM,   0 },
    { "dhcp.",            AID_SYSTEM,   0 },
    { "dhcp.",            AID_DHCP,     0 },
    { "debug.",           AID_SHELL,    0 },
    { "log.",             AID_SHELL,    0 },
    { "service.adb.root", AID_SHELL,    0 },
    { "service.adb.tcp.port", AID_SHELL,    0 },
    { "persist.sys.",     AID_SYSTEM,   0 },
    { "persist.service.", AID_SYSTEM,   0 },
    { "persist.service.", AID_RADIO,    0 },
    { "persist.security.",AID_SYSTEM,   0 },
    { "net.pdp",          AID_RADIO,    AID_RADIO },
    { "watchdog.reset_progress",      AID_RADIO,    AID_RADIO },
    { "media.tegra",      AID_MEDIA,    0 },
    { NULL, 0, 0 }
};

