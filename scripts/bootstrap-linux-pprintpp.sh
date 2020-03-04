rm -rf build
mkdir build

cd build && cmake -DNANOLOG_USE_PPRINTPP=On \
  ..


