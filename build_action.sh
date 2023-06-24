#!/usr/bin/env bash

VERSION=$(grep 'Kernel Configuration' < config | awk '{print $3}')
# add deb-src to sources.list
sed -i "/deb-src/s/# //g" /etc/apt/sources.list

# install dep
sudo apt update
sudo apt install -y wget xz-utils make gcc flex bison dpkg-dev bc rsync kmod cpio libssl-dev git lsb vim libelf-dev
sudo apt build-dep -y linux

# change dir to workplace
cd "${GITHUB_WORKSPACE}" || exit

# download kernel source
wget http://www.kernel.org/pub/linux/kernel/v4.x/linux-$VERSION.tar.gz  
tar -xf linux-"$VERSION".tar.gz
cd linux-"$VERSION" || exit

# copy config file
cp ../config .config
#
# disable DEBUG_INFO to speedup build
scripts/config --disable DEBUG_INFO
scripts/config --set-str SYSTEM_TRUSTED_KEYS ""
# apply patches
# shellcheck source=src/util.sh
# source ../patch.d/*.sh

# build deb packages
CPU_CORES=$(($(grep -c processor < /proc/cpuinfo)*2))
sudo make bindeb-pkg -j"$CPU_CORES"

# move deb packages to artifact dir
cd ..
mkdir "artifact"
#cp ./*.deb artifact/
git clone https://gfdgd-xi:$PASSWORD@github.com/gfdgd-xi/dclc-kernel
#cd dclc-kernel
mkdir dclc-kernel/$VERSION
rm -rfv *dbg*.deb
mv ./*.deb dclc-kernel/$VERSION
cd dclc-kernel/$VERSION
bash ./repack-zstd --scan ..
./build.py
git add .
git config --global user.email 3025613752@qq.com
git config --global user.name gfdgd-xi
git commit -m 提交$VERSION
git push