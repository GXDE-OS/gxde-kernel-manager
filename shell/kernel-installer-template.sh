#!/bin/bash
set -e
rm /tmp/gxde-kernel-manager-installer-status -f
apt update
apt install ${KernelList} -y
rm -f "${kernelInstallerShellTempPath}"