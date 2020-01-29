## Please edit android sdk/ndk/cmake path
ANDROID_SDK_ROOT=$HOME/Android/Sdk/
#ANDROID_NDK_ROOT=$HOME/Android/Sdk/ndk-bundle
ANDROID_NDK_ROOT=$HOME/Android/Sdk/ndk/21.0.6113669/
CMAKE_BIN=$ANDROID_SDK_ROOT/cmake/3.10.2.4988404/bin/cmake
# CMake 3.7 or later required
#CMAKE_BIN=cmake

rm -rf build-android-standalone

# For ninja build
# -DCMAKE_MAKE_PROGRAM=$ANDROID_SDK_ROOT/cmake/3.6.4111459/bin/ninja \

mkdir build-android-standalone
cd build-android-standalone && $CMAKE_BIN \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_ROOT/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_NATIVE_API_LEVEL=28 \
  -DANDROID_STL=c++_shared \
  -DNANOLOG_ANDROID_USE_STDIO=On \
  ..


