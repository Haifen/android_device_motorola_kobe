# Copyright (C) 2011 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

DEVICE_PREBUILT := device/motorola/kobe/prebuilt

#key layouts, names must fit the ones in /proc/bus/input/devices, qwerty.kl is the fallback one.
PRODUCT_COPY_FILES += \
	device/motorola/kobe/prebuilt/usr/qwerty.kl:system/usr/keylayout/qwerty.kl \
	device/motorola/kobe/prebuilt/usr/qwerty.kl:system/usr/keylayout/qtouch-touchscreen.kl \
	device/motorola/kobe/prebuilt/usr/keypad.kl:system/usr/keylayout/sholes-keypad.kl \
	device/motorola/kobe/prebuilt/usr/keypad.kl:system/usr/keylayout/umts_kobe-keypad.kl \
	device/motorola/kobe/prebuilt/usr/cpcap.kl:system/usr/keylayout/cpcap-key.kl \

#etc
PRODUCT_COPY_FILES += \
	device/motorola/kobe/bootmenu/recovery/recovery.fstab:system/etc/recovery.fstab \
	device/motorola/kobe/prebuilt/etc/init.d/01sysctl:system/etc/init.d/01sysctl \
	device/motorola/kobe/prebuilt/etc/init.d/03adbd:system/etc/init.d/03adbd \
	device/motorola/kobe/prebuilt/etc/init.d/05mountsd:system/etc/init.d/05mountsd \
	device/motorola/kobe/prebuilt/etc/init.d/08backlight:system/etc/init.d/08backlight \
	device/motorola/kobe/prebuilt/etc/init.d/90multitouch:system/etc/init.d/90multitouch \
	device/motorola/kobe/prebuilt/etc/profile:system/etc/profile \
	device/motorola/kobe/prebuilt/etc/sysctl.conf:system/etc/sysctl.conf \
	device/motorola/kobe/prebuilt/etc/busybox.fstab:system/etc/fstab \
	device/motorola/kobe/prebuilt/etc/wifi/dnsmasq.conf:system/etc/wifi/dnsmasq.conf \
	device/motorola/kobe/prebuilt/etc/wifi/tiwlan.ini:system/etc/wifi/tiwlan.ini \
	device/motorola/kobe/prebuilt/etc/wifi/tiwlan_ap.ini:system/etc/wifi/tiwlan_ap.ini \
	device/motorola/kobe/prebuilt/etc/wifi/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf \
	device/motorola/kobe/prebuilt/etc/gpsconfig.xml:system/etc/gpsconfig.xml \
	device/motorola/kobe/prebuilt/etc/location.cfg:system/etc/location.cfg \
	device/motorola/kobe/media_profiles.xml:system/etc/media_profiles.xml \
	device/motorola/kobe/modules/modules.alias:system/lib/modules/modules.alias \
	device/motorola/kobe/modules/modules.dep:system/lib/modules/modules.dep \

ifdef CYANOGEN_RELEASE
	PRODUCT_COPY_FILES += device/motorola/kobe/custom_backup_release.txt:system/etc/custom_backup_list.txt
else
	PRODUCT_COPY_FILES += device/motorola/kobe/custom_backup_list.txt:system/etc/custom_backup_list.txt
endif

#app
PRODUCT_COPY_FILES += \
	device/motorola/kobe/prebuilt/app/basebandswitcherV4.0.apk:system/app/basebandswitcherV4.0.apk \
	device/motorola/kobe/prebuilt/app/DroidSSHd.apk:system/app/DroidSSHd.apk \
	device/motorola/kobe/prebuilt/lib/libNativeSSHd.so:system/lib/libNativeSSHd.so \

#end of kobe-blobs.mk
