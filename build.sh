#!/bin/bash - 
#===============================================================================
#
#          FILE: build.sh
# 
#         USAGE: ./build.sh 
# 
#   DESCRIPTION: 
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: wanmingxiang (wanmx@foxmail.com), 
#  ORGANIZATION: 
#       CREATED: 2017/05/10 01:15
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error

# build steps
if [ -d ./build ]; then
	rm -rf ./build
fi
mkdir build && cd ./build
cmake -DCMAKE_INSTALL_PREFIX=./ ..
make -s
make install
