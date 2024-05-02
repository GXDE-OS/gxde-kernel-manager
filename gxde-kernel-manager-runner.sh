#!/bin/bash
set -e
programPath=$(cd $(dirname $0); pwd)
arch=$(dpkg --print-architecture)
if [[ -d "$programPath/lib/$arch" ]]; then
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$programPath/lib/$arch
fi
$programPath/gxde-kernel-manager