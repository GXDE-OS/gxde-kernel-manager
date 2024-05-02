#!/bin/bash
set -e
if [[ ! -n $1 ]]; then
    echo "Error, option isn't full"
    exit 1
fi

programPath=$(cd $(dirname $0); pwd)
dpkg -x "$1" /tmp/gxde-kernel-manager
dpkg -e "$1" /tmp/gxde-kernel-manager/DEBIAN
cp $programPath/lib /tmp/gxde-kernel-manager/opt/apps/gxde-kernel-manager/ -rv
if [[ ! -n $2 ]]; then
    dpkg-deb -Z xz -z 9 -b /tmp/gxde-kernel-manager "$1"
else
    dpkg-deb -Z xz -z 9 -b /tmp/gxde-kernel-manager "$2"
fi