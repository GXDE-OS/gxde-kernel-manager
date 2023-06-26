#!/usr/bin/env python3
import os
import sys
import pyquery

def CompareVersion(old, new):
    oldList = old.split(".")
    newList = new.split(".")
    for i in range(len(oldList)):
        oldN = int(oldList[i])
        try:
            newN = int(newList[i])
        except:
            newN = 0
        if newN > oldN:
            return 1
        if newN < oldN:
            return -1
    return 0

version=sys.argv[1]
newestVersion = "0.0.0"
newestUrl = ""
programVersionList = pyquery.PyQuery(url=f"https://cdn.kernel.org/pub/linux/kernel/v{version}.x/")
for i in programVersionList("a").items():
    #continue
    url = i.attr.href
    if url[-7:] == ".tar.xz" and url[:6] == "linux-":
        nowversion = os.path.basename(url).lower().replace("linux-", "").replace(".tar.xz", "")
        if "-" in nowversion:
            nowversion = nowversion[:nowversion.index("-")]
        print(newestUrl)
        if CompareVersion(newestVersion, nowversion) == 1:
            newestVersion = nowversion
            newestUrl = url
print(newestVersion)
newestUrl = f"https://cdn.kernel.org/pub/linux/kernel/v{version}.x/{newestUrl}"
print(newestUrl)
with open("/tmp/kernelversion.txt", "w") as file:
    file.write(newestVersion)
with open("/tmp/kernelurl.txt", "w") as file:
    file.write(newestUrl)