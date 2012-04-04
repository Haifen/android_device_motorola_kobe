# This script is included in squisher
# It is the final build step (after OTA package)

# set in squisher
# DEVICE_OUT=$ANDROID_BUILD_TOP/out/target/product/kobe
# DEVICE_TOP=$ANDROID_BUILD_TOP/device/motorola/kobe
# VENDOR_TOP=$ANDROID_BUILD_TOP/vendor/motorola/kobe

# Delete unwanted apps
rm -f $REPACK/ota/system/app/RomManager.apk
rm -f $REPACK/ota/system/app/FOTAKill.apk
rm -f $REPACK/ota/system/app/Protips.apk
rm -f $REPACK/ota/system/xbin/irssi

# Delete unused libs
rm -f $REPACK/ota/system/lib/hw/*.goldfish.so
rm -f $REPACK/ota/system/lib/hw/gralloc.default.so

# these scripts are not required
rm $REPACK/ota/system/etc/init.d/03firstboot
rm $REPACK/ota/system/etc/init.d/04modules

# add an empty script to prevent logcat errors (moto init.rc)
touch $REPACK/ota/system/bin/mount_ext3.sh
chmod +x $REPACK/ota/system/bin/mount_ext3.sh
	
mkdir -p $REPACK/ota/system/etc/terminfo/x
cp $REPACK/ota/system/etc/terminfo/l/linux $REPACK/ota/system/etc/terminfo/x/xterm

# prebuilt boot, devtree, logo & updater-script
rm -f $REPACK/ota/boot.img

cp -f $DEVICE_TOP/updater-script $REPACK/ota/META-INF/com/google/android/updater-script

cp -f $DEVICE_OUT/root/init $REPACK/ota/system/bootmenu/2nd-init/init
cp -f $DEVICE_OUT/root/init.rc $REPACK/ota/system/bootmenu/2nd-init/init.rc
cp -f $DEVICE_OUT/root/sbin/adbd $REPACK/ota/system/bin/adbd

# use the static busybox in bootmenu, this helps a lot on incomplete roms and ics
cp -f $DEVICE_OUT/utilities/busybox $REPACK/ota/system/bootmenu/binary/busybox

