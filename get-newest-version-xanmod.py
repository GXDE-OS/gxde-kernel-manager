#!/usr/bin/env python3
import os
import sys
import pyquery
mainVersion = int(sys.argv[1])
programVersionList = pyquery.PyQuery(url=f"https://www.xanmod.org/")
#release = pyquery.PyQuery()
temp = 0
newestVersion = "0.0.0"
newestUrl = ""
for i in programVersionList(f"#content div.page div.level3 :first-child td a").items():
    print(i)
    if ".tar" in i.attr.href:
        if temp == mainVersion:
            newestVersion = os.path.splitext(os.path.splitext(os.path.basename(i.attr.href))[0])[0]
            newestUrl = i.attr.href
            break
    #print(i)
for i in programVersionList(f"#content div.page div.level3 table a").items():
    break
    print(i)
    continue
    version = i("td strong").text()
    if temp == mainVersion:
        #print(version)
        newestVersion = version
        break
    temp += 1

temp = 0
for i in programVersionList(f"#releases tr :nth-child(4)").items():
    url = i("td a").attr.href
    if temp == mainVersion:
        newestUrl = url
        break
    temp += 1
print(newestVersion)

print(newestUrl)
with open("/tmp/kernelversion.txt", "w") as file:
    file.write(newestVersion)
with open("/tmp/kernelurl.txt", "w") as file:
    file.write(newestUrl)
