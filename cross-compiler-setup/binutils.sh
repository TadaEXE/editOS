#!/usr/bin/env bash
set -euo pipefail

TMP_LD_LIBRARY_PATH=$LD_LIBRARY_PATH
unset LD_LIBRARY_PATH
TMP_CFLAGS=$CFLAGS
unset CFLAGS
TMP_CXXFLAGS=$CXXFLAGS
unset CXXFLAGS

# Versions (can be overridden via env)
BINUTILS_VER=${BINUTILS_VER:-2.45.1}
GCC_VER=${GCC_VER:-15.2.0}
GMP_VER=${GMP_VER:-6.3.0}
MPFR_VER=${MPFR_VER:-4.2.2}
MPC_VER=${MPC_VER:-1.3.1}

# Target and install prefix
TARGET=${TARGET:-x86_64-elf}
PREFIX=${PREFIX:-$PWD/cross-compiler-setup/toolchain}

# Where the tarballs from ftp.gnu.org live
SRC_ROOT=${SRC_ROOT:-"$PWD/cross-compiler-setup/.download"}

# Where to put build directories
BUILD_ROOT=${BUILD_ROOT:-"$PWD/cross-compiler-setup/.build"}

JOBS=${JOBS:-$(($(command -v nproc >/dev/null 2>&1 && nproc || echo 1)/2))}

require_tarball() {
  local f="$1"
  if [ ! -f "$SRC_ROOT/$f" ]; then
    echo "Missing source tarball: $SRC_ROOT/$f" >&2
    echo "Run the download script first or place it there manually." >&2
    exit 1
  fi
}

extract_if_missing() {
  local tar="$1"
  local dir="$2"
  if [ ! -d "$SRC_ROOT/$dir" ]; then
    echo "Extracting $tar"
    (cd "$SRC_ROOT" && tar xf "$tar")
  else
    echo "Using existing directory: $SRC_ROOT/$dir"
  fi
}

echo "Using SRC_ROOT=$SRC_ROOT"
echo "Using BUILD_ROOT=$BUILD_ROOT"
echo "Using PREFIX=$PREFIX"
echo "Using TARGET=$TARGET"

require_tarball "binutils-$BINUTILS_VER.tar.xz"
require_tarball "gcc-$GCC_VER.tar.xz"
require_tarball "gmp-$GMP_VER.tar.xz"
require_tarball "mpfr-$MPFR_VER.tar.xz"
require_tarball "mpc-$MPC_VER.tar.gz"

extract_if_missing "binutils-$BINUTILS_VER.tar.xz" "binutils-$BINUTILS_VER"
extract_if_missing "gcc-$GCC_VER.tar.xz" "gcc-$GCC_VER"
extract_if_missing "gmp-$GMP_VER.tar.xz" "gmp-$GMP_VER"
extract_if_missing "mpfr-$MPFR_VER.tar.xz" "mpfr-$MPFR_VER"
extract_if_missing "mpc-$MPC_VER.tar.gz" "mpc-$MPC_VER"

echo "Preparing in-tree GMP/MPFR/MPC in GCC source"

cd "$SRC_ROOT/gcc-$GCC_VER"

if [ ! -d gmp ]; then
  ln -s "../gmp-$GMP_VER" gmp
fi

if [ ! -d mpfr ]; then
  ln -s "../mpfr-$MPFR_VER" mpfr
fi

if [ ! -d mpc ]; then
  ln -s "../mpc-$MPC_VER" mpc
fi

echo "Building binutils"

BUILD_BINUTILS="${BUILD_ROOT}/build-binutils-${TARGET}"
mkdir -p "$BUILD_BINUTILS"
cd "$BUILD_BINUTILS"

"$SRC_ROOT/binutils-$BINUTILS_VER/configure" \
  --target="$TARGET" \
  --prefix="$PREFIX" \
  --with-sysroot \
  --disable-nls \
  --disable-werror

make -j"$JOBS"
make install

echo "Building GCC (freestanding, no libc, with freestanding libstdc++)"

BUILD_GCC="${BUILD_ROOT}/build-gcc-${TARGET}"
mkdir -p "$BUILD_GCC"
cd "$BUILD_GCC"

"$SRC_ROOT/gcc-$GCC_VER/configure" \
  --target="$TARGET" \
  --prefix="$PREFIX" \
  --disable-nls \
  --enable-languages=c,c++ \
  --without-headers \
  --disable-hosted-libstdcxx

make all-gcc -j"$JOBS"
make all-target-libgcc -j"$JOBS"
make all-target-libstdc++-v3 -j"$JOBS"

make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3

echo
echo "Done."
echo "Cross compiler installed to: $PREFIX"
echo "Try:  x86_64-elf-g++ -std=gnu++20 -ffreestanding -c test.cpp"

export LD_LIBRARY_PATH=$TMP_LD_LIBRARY_PATH
export CFLAGS=$TMP_CFLAGS
export CXXFLAGS=$TMP_CXXFLAGS
