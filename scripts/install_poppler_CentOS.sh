#!/bin/bash 
# Build Poppler library in CentOS

function isinstalled {
  if yum list installed "$@" >/dev/null 2>&1; then
    true
  else
    false
  fi
}

deps="freetype-devel fontconfig-devel libjpeg-devel libpng-devel libtiff-devel openjpeg2-devel lcms2-devel libcurl-devel mesa-libGL-devel"
popplerLink=https://poppler.freedesktop.org/poppler-0.65.0.tar.xz
proc=$(cat /proc/cpuinfo | grep "processor" | wc -l)
echo -e "\033[31m You need to configure Qt5 properly to build poppler. \033[0m"
# fetch gcc-7 and cmake
echo -e "\033[36m Fetch gcc-7 and cmake ...\033[0m"
repos="centos-release-scl epel-release"
if isinstalled $repos; then 
	echo " $repos already installed";
else
	sudo yum install -y $repos
	sudo yum clean all && yum makecache
fi

toolchain="llvm-toolset-7-cmake devtoolset-7"

if isinstalled $toolchain; then 
	echo " $toolchain already installed";
else
	sudo yum install -y $toolchain
fi
source scl_source enable devtoolset-7
source scl_source enable llvm-toolset-7
#poppler Lib
set -e
echo -e "\033[36m Fetch Poppler source code ... \033[0m"
wget -O poppler.tar.xz $popplerLink
tar -xvf poppler.tar.xz
# install deps
set +e
echo -e "\033[36m Install Dependencies ... \033[0m"
sudo yum install -y $deps
set -e
echo -e "\033[36m Start to build the library ... \033[0m"
cd poppler* && mkdir build
# compile the library, automatically using multi-proc
cd build && cmake .. && make -j$proc
# install the library & set the environment vars
echo "LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:/usr/local/lib64:/usr/local/lib"|sudo tee /etc/profile.d/poppler.sh
sudo make install
cd ../..
rm -rf poppler*