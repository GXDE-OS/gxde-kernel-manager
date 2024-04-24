#!/bin/bash
rm /tmp/gxde-kernel-manager-installer-status -f
pkexec bash "${kernelInstallerShellTempPath}"
echo $? > /tmp/gxde-kernel-manager-installer-status
rm -f "${kernelInstallerRootLoaderShellTempPath}"