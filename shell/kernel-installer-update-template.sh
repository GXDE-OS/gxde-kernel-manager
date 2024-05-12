#!/bin/bash
set -e
rm /tmp/gxde-kernel-manager-installer-status -f
aptPath="apt"
which aptss > /dev/null
if [[ $? == 0 ]]; then
    # 如果 aptss 存在，则使用 aptss
    aptPath="aptss"
fi
$aptPath update
rm -f "${kernelInstallerShellTempPath}"