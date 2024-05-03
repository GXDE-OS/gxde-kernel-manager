#!/bin/bash
set -e
if [[ ! -n $1 ]]; then
    echo "Error, option isn't full"
    exit 1
fi

programPath=$(cd $(dirname $0); pwd)
dpkg -x "$1" /tmp/gxde-kernel-manager
dpkg -e "$1" /tmp/gxde-kernel-manager/DEBIAN
sed -i "s/libqtermwidget5-0 (>= 0.8.0), /libqtermwidget5-0 | libqtermwidget5-1, /g" /tmp/gxde-kernel-manager/DEBIAN/control
cp $programPath/lib /tmp/gxde-kernel-manager/opt/apps/gxde-kernel-manager/ -rv
if [[ ! -n $2 ]]; then
    dpkg-deb -Z xz -z 9 -b /tmp/gxde-kernel-manager "$1"
else
    dpkg-deb -Z xz -z 9 -b /tmp/gxde-kernel-manager "$2"
fi