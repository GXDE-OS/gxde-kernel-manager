#!/bin/bash
set -e
rm /tmp/gxde-kernel-manager-installer-status -f
aptPath="apt"
if [[ -f "/usr/bin/aptss" ]]; then
    # 如果 aptss 存在，则使用 aptss
    aptPath="aptss"
fi
$aptPath update
$aptPath install ${KernelList} -y
rm -f "${kernelInstallerShellTempPath}"
