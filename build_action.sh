#!/usr/bin/env bash
sed -i "/deb-src/s/# //g" /etc/apt/sources.list
sudo apt update
sudo apt install python3-pyquery -y
python3 get-newest-version.py $1
#VERSION=$(grep 'Kernel Configuration' < config | awk '{print $3}')
# add deb-src to sources.list

VERSION=`cat /tmp/kernelversion.txt`
URL=`cat /tmp/kernelurl.txt`
curl https://github.com/gfdgd-xi/dclc-kernel/raw/main/$VERSION/index.html | grep 404
if [[ $? != 0 ]]; then
    exit
fi
# install dep
sudo apt install -y wget xz-utils make gcc flex bison dpkg-dev bc rsync kmod cpio libssl-dev git lsb vim libelf-dev
sudo apt build-dep -y linux

# change dir to workplace
cd "${GITHUB_WORKSPACE}" || exit

# download kernel source
wget $URL  
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
cd ..
cd head
cat > deb/DEBIAN/control <<EOF
Package: linux-kernel-dclc-gfdgdxi
Version: $VERSION
Maintainer: gfdgd xi <3025613752@qq.com>
Homepage: https://github.com/gfdgd-xi/dclc-kernel
Architecture: amd64
Severity: serious
Certainty: possible
Check: binaries
Type: binary, udeb
Priority: optional
Depends: linux-headers-$VERSION-amd64-desktop, linux-image-$VERSION-amd64-desktop
Section: utils
Installed-Size: 0
Description: 内核（虚包）
EOF
dpkg -b deb linux-kernel-dclc-gfdgdxi_${VERSION}_amd64.deb
cd ..
bash ./repack-zstd --scan .
./build.py
git add .
git config --global user.email 3025613752@qq.com
git config --global user.name gfdgd-xi
git commit -m 提交$VERSION
git push