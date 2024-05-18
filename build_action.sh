#!/usr/bin/env bash
sudo rm -rfv *
sudo rm -rfv /tmp/*
sed -i "/deb-src/s/# //g" /etc/apt/sources.list
sudo apt update
sudo apt install gpg python3-pyquery -y
aria2c $KEY
gpg --import  --pinentry-mode loopback --batch --passphrase "$KEYPASSWORD"  private-file.key
python3 get-newest-version.py $1
#VERSION=$(grep 'Kernel Configuration' < config | awk '{print $3}')
# add deb-src to sources.list
VERSION=`cat /tmp/kernelversion.txt`
URL=`cat /tmp/kernelurl.txt`
MAINVERSION=`expr substr $VERSION 1 1`
SHOWVERSION=$VERSION
# 使用 deepin hwe config编译
if [[ $2 == 1 ]]; then
    SHOWVERSION=$VERSION-hwe
fi
if [[ $2 == 1 ]]; then
    curl https://github.com/gfdgd-xi/gxde-linux-kernel/raw/main/l/linux-kernel-gxde-gfdgdxi-$1-hwe/linux-kernel-gxde-gfdgdxi-$1-hwe_${VERSION}_amd64.deb | grep 404
    result=$?
else
    curl https://github.com/gfdgd-xi/gxde-linux-kernel/raw/main/l/linux-kernel-gxde-gfdgdxi-$1/linux-kernel-gxde-gfdgdxi-${1}_${VERSION}_amd64.deb | grep 404
    result=$?
fi
if [[ $result != 0 ]]; then
    exit
fi
# install dep
sudo apt install -y wget xz-utils make gcc flex bison dpkg-dev bc rsync kmod cpio libssl-dev git lsb vim libelf-dev
sudo apt build-dep -y linux

# change dir to workplace
cd "${GITHUB_WORKSPACE}" || exit

# download kernel source
wget $URL  
if [[ -f linux-"$VERSION".tar.xz ]]; then
    tar -xf linux-"$VERSION".tar.xz
fi
if [[ -f linux-"$VERSION".tar.gz ]]; then
    tar -xf linux-"$VERSION".tar.gz
fi
if [[ -f linux-"$VERSION".tar ]]; then
    tar -xf linux-"$VERSION".tar
fi
if [[ -f linux-"$VERSION".bz2 ]]; then
    tar -xf linux-"$VERSION".tar.bz2
fi
cd linux-"$VERSION" || exit

# copy config file
# 使用 deepin hwe config编译
if [[ $2 == 1 ]]; then
    cp ../config-6.1.11-amd64-desktop-hwe .config
else
    cp ../config .config
fi
echo $VERSION | grep 4.14
if [[ $? == 0 ]]; then
    cp ../config-4.19.0-10-amd64 .config
fi
#
# disable DEBUG_INFO to speedup build
# scripts/config --disable DEBUG_INFO 
scripts/config --set-str SYSTEM_TRUSTED_KEYS ""
scripts/config --set-str SYSTEM_REVOCATION_KEYS ""
scripts/config --undefine DEBUG_INFO
scripts/config --undefine DEBUG_INFO_COMPRESSED
scripts/config --undefine DEBUG_INFO_REDUCED
scripts/config --undefine DEBUG_INFO_SPLIT
scripts/config --undefine GDB_SCRIPTS
scripts/config --disable DEBUG_INFO
scripts/config --set-val  DEBUG_INFO_DWARF5     n
scripts/config --set-val  DEBUG_INFO_NONE       y

# build deb packages
CPU_CORES=$(($(grep -c processor < /proc/cpuinfo)*2))
sudo env DEBEMAIL="gfdgd xi <3025613752@qq.com>" make bindeb-pkg -j"$CPU_CORES"
# 移动 deb
mkdir /tmp/deb-result
cp ../*.deb /tmp/deb-result
rm /tmp/deb-result/linux-libc-dev*.deb
rm /tmp/deb-result/*dbg*.deb

if [[ $2 == 1 ]]; then
    # move deb packages to artifact dir
    cd ..
    mkdir "artifact"
    #cp ./*.deb artifact/
    rm -rfv "${GITHUB_WORKSPACE}/linux-$VERSION*"
    git clone https://gfdgd-xi:$PASSWORD@github.com/gfdgd-xi/gxde-linux-kernel --depth=1
    mkdir dclc-kernel
    mkdir dclc-kernel/$SHOWVERSION
    mkdir dclc-kernel/head
    rm -rfv *dbg*.deb
    mv ./*.deb dclc-kernel/$SHOWVERSION
    cd dclc-kernel/$SHOWVERSION
    cd ..
    cd head
    mkdir deb/DEBIAN -vp
    cat > deb/DEBIAN/control <<EOF
Package: linux-kernel-gxde-gfdgdxi-hwe
Version: $VERSION
Maintainer: gfdgd xi <3025613752@qq.com>
Homepage: https://github.com/gfdgd-xi/dclc-kernel
Architecture: amd64
Severity: serious
Certainty: possible
Check: binaries
Type: binary, udeb
Priority: optional
Depends: linux-headers-$VERSION-amd64-gfdgdxi-desktop-hwe, linux-image-$VERSION-amd64-gfdgdxi-desktop-hwe
Section: utils
Installed-Size: 0
Description: 内核 $VERSION
EOF
    if [[ ! -d deb-$MAINVERSION ]]; then
        mkdir -pv deb-$MAINVERSION/DEBIAN
    fi
    cat > deb-$MAINVERSION/DEBIAN/control <<EOF
Package: linux-kernel-gxde-gfdgdxi-$1-hwe
Version: $VERSION
Maintainer: gfdgd xi <3025613752@qq.com>
Homepage: https://github.com/gfdgd-xi/dclc-kernel
Architecture: amd64
Severity: serious
Certainty: possible
Check: binaries
Type: binary, udeb
Priority: optional
Depends: linux-headers-$VERSION-amd64-gfdgdxi-desktop-hwe, linux-image-$VERSION-amd64-gfdgdxi-desktop-hwe
Section: utils
Installed-Size: 0
Description: 内核 HWE $VERSION
EOF
    dpkg-deb -Z xz -b deb linux-kernel-gxde-gfdgdxi-hwe_${VERSION}_amd64.deb
    dpkg-deb -Z xz -b deb-$MAINVERSION linux-kernel-gxde-gfdgdxi-$1-hwe_${VERSION}_amd64.deb
    cd ../../gxde-linux-kernel
    #bash ./repack-zstd --scan .
    #./build.py
    ./move-letter-path.py ../dclc-kernel/$SHOWVERSION/*.deb
    ./move-letter-path.py ../dclc-kernel/head/*.deb
    ./move-letter-path.py /tmp/deb-result/*.deb
    touch l/linux-headers-$VERSION-amd64-gfdgdxi-desktop-hwe/lock
    rm -rfv l/linux-libc-dev/
    git add .
    #git pull
    git config --global user.email 3025613752@qq.com
    git config --global user.name gfdgd-xi
    git commit -m 提交$VERSION
    git push
else
    # move deb packages to artifact dir
    cd ..
    mkdir "artifact"
    #cp ./*.deb artifact/
    rm -rfv "${GITHUB_WORKSPACE}/linux-$VERSION*"
    git clone https://gfdgd-xi:$PASSWORD@github.com/gfdgd-xi/gxde-linux-kernel --depth=1
    #cd dclc-kernel
    mkdir dclc-kernel
    mkdir dclc-kernel/$VERSION
    mkdir dclc-kernel/head
    rm -rfv *dbg*.deb
    mv ./*.deb dclc-kernel/$VERSION
    cd dclc-kernel/$VERSION
    cd ..
    cd head
    mkdir deb/DEBIAN -vp
    cat > deb/DEBIAN/control <<EOF
Package: linux-kernel-gxde-gfdgdxi
Version: $VERSION
Maintainer: gfdgd xi <3025613752@qq.com>
Homepage: https://github.com/gfdgd-xi/dclc-kernel
Architecture: amd64
Severity: serious
Certainty: possible
Check: binaries
Type: binary, udeb
Priority: optional
Depends: linux-headers-$VERSION-amd64-gfdgdxi-desktop, linux-image-$VERSION-amd64-gfdgdxi-desktop
Section: utils
Installed-Size: 0
Description: 内核（虚包）
EOF
    if [[ ! -d deb-$MAINVERSION ]]; then
        mkdir -pv deb-$MAINVERSION/DEBIAN
    fi
    cat > deb-$MAINVERSION/DEBIAN/control <<EOF
Package: linux-kernel-gxde-gfdgdxi-$1
Version: $VERSION
Maintainer: gfdgd xi <3025613752@qq.com>
Homepage: https://github.com/gfdgd-xi/dclc-kernel
Architecture: amd64
Severity: serious
Certainty: possible
Check: binaries
Type: binary, udeb
Priority: optional
Depends: linux-headers-$VERSION-amd64-gfdgdxi-desktop, linux-image-$VERSION-amd64-gfdgdxi-desktop
Section: utils
Installed-Size: 0
Description: 内核（虚包）
EOF
    dpkg-deb -Z xz -b deb linux-kernel-gxde-gfdgdxi_${VERSION}_amd64.deb
    dpkg-deb -Z xz -b deb-$MAINVERSION linux-kernel-gxde-gfdgdxi-${1}_${VERSION}_amd64.deb
    cd ../../gxde-linux-kernel
    #bash ./repack-zstd --scan .
    #./build.py
    ./move-letter-path.py ../dclc-kernel/$VERSION/*.deb
    ./move-letter-path.py ../dclc-kernel/head/*.deb
    ./move-letter-path.py /tmp/deb-result/*.deb
    rm -rfv l/linux-libc-dev/
    touch l/linux-headers-$VERSION-amd64-gfdgdxi-desktop/lock
    git add .
    #git pull
    git config --global user.email 3025613752@qq.com
    git config --global user.name gfdgd-xi
    git commit -m 提交$VERSION
    git push
fi
