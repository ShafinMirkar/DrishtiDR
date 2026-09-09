Linux
sudo apt update
sudo apt install -y build-essential cmake git pkg-config qt6-base-dev qt6-base-dev-tools

git clone https://github.com/<USERNAME>/DrishtiDR.git
cd DrishtiDR

cmake -S . -B build \
    -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6 \
    -DCMAKE_BUILD_TYPE=Release

cmake --build build -j$(nproc)

./build/DrishtiDx



Windows, MSYS2 UCRT64
pacman -Syu --noconfirm

pacman -S --needed --noconfirm \
    mingw-w64-ucrt-x86_64-toolchain \
    mingw-w64-ucrt-x86_64-cmake \
    mingw-w64-ucrt-x86_64-ninja \
    mingw-w64-ucrt-x86_64-qt6-base

git clone https://github.com/<USERNAME>/DrishtiDR.git
cd DrishtiDR

cmake -S . -B build \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release

cmake --build build

./build/DrishtiDx.exe
