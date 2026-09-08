rebuild cleanly
cmake -S . -B build \
  -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6

cmake --build build

Run:

./build/DrishtiDx
