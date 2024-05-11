#!/bin/bash
set -e
rm /tmp/gxde-kernel-manager-installer-status -f
dpkg-reconfigure ${KernelList} 
rm -f "${kernelInstallerShellTempPath}"