#!/sbin/sh
# Free some system space by clearing non-essentials for p99x
#

case "$1" in
  backup)
    if [ -d /system/usr/srec/en-US ] ; then
       mount /data/
       if [ ! -d /cache/srec ]; then
           mv /system/usr/srec/en-US /cache/srec
           ln -s /cache/srec /system/usr/srec/en-US
       fi
       umount /data
    fi
  ;;
  restore)
    if [ ! -d /cache/srec ]; then
       mkdir -p /cache/srec
    fi
    ln -s /cache/srec /system/usr/srec/en-US
  ;;
esac
