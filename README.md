<p width=100px align="center"><img width=40% src="icon/icon.svg"></p>
<h1 align="center">GXDE 内核管理器 1.0.0</h1>
<hr>
<a href='https://gitee.com/GXDE-OS/gxde-kernel-manager/stargazers'><img src='https://gitee.com/GXDE-OS/gxde-kernel-manager/badge/star.svg?theme=dark' alt='star'></img></a>
<a href='https://gitee.com/GXDE-OS/gxde-kernel-manager/members'><img src='https://gitee.com/GXDE-OS/gxde-kernel-manager/badge/fork.svg?theme=dark' alt='fork'></img></a>  

## 介绍
GXDE 内核管理器是一个帮助用户更方便获取、安装、移除内核的工具。  
目前支持 amd64、arm64、mips64 和 loong64 四个架构  
***警告：请不要随意操作内核除非你知道你在做什么！***  


## 内核仓库 apt 源
安装 GXDE 内核管理器后会自动添加  
源地址：https://kernel.gxde.gfdgdxi.top/  

## 编译安装
```bash
git clone https://gitee.com/GXDE-OS/gxde-kernel-manager
cd gxde-kernel-manager
sudo apt build-dep .
qmake .
make -j4
sudo make install -j4
```
## 构建 deb 包
```bash
git clone https://gitee.com/GXDE-OS/gxde-kernel-manager
cd gxde-kernel-manager
sudo apt build-dep .
sudo apt install fakeroot
dpkg-buildpackage -b -us
```

# ©2023～Now gfdgd xi