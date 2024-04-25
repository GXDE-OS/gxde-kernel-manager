#!/bin/bash
set -e
rm /tmp/gxde-kernel-manager-installer-status -f
apt purge ${KernelList} -y
rm -f "${kernelInstallerShellTempPath}"