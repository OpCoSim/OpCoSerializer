#!/bin/sh

# Installs OpCoSerializer dependencies. It is assumed that
# required toolchain programs such as cmake, gcc etc. are
# already available.

RAPIDJSON_VERSION="1.1.0"
RAPIDJSON_ARCHIVE="v$RAPIDJSON_VERSION.tar.gz"
RAPIDJSON_DIRECTORY="rapidjson-$RAPIDJSON_VERSION"

mkdir -p ThirdParty/include
cd ThirdParty
rm -rf *
wget "https://github.com/Tencent/rapidjson/archive/refs/tags/$RAPIDJSON_ARCHIVE"
tar -xvzf $RAPIDJSON_ARCHIVE
cp -r "./$RAPIDJSON_DIRECTORY/include" ./include
rm -rf $RAPIDJSON_DIRECTORY $RAPIDJSON_ARCHIVE
cd ..
