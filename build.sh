PROJECT=equimoves
BUILD=build-linux
mkdir -p $BUILD
cd $BUILD
INSTALL=../install-linux
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL -DLCC_PROJECT_NAME=$PROJECT
make
make install
