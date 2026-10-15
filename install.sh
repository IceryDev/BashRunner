#!/bin/bash
set -e

source /etc/os-release
pkgs=(extra-cmake-modules
          cmake
          ninja
          qt6-base
          ki18n
          kconfig
          krunner
          kio
          knotifications)
missing=()

kversion=$(krunner --version | grep -oP "(?<=krunner )\d+" || echo "")
if [ ! "$kversion" == "6" ]
then
  echo "Unsupported KRunner version: $ID"
  exit 1
fi

check_arch() {
  for pkg in "${pkgs[@]}"; do
      if ! pacman -Qi "$pkg" &> /dev/null; then
          missing+=("$pkg")
      fi
  done
}

check_debian() {
  for pkg in "${pkgs[@]}"; do
      if ! dpkg -s "$pkg" &> /dev/null; then
          missing+=("$pkg")
      fi
  done
}

check_fedora() {
  for pkg in "${pkgs[@]}"; do
      if ! rpm -q "$pkg" &> /dev/null; then
          missing+=("$pkg")
      fi
  done
}

case "$ID" in
    arch)
      check_fn=check_arch
      install_cmd="sudo pacman -S --needed"
      ;;
    debian|ubuntu)
      check_fn=check_debian
      install_cmd="sudo apt-get install -y"
      ;;
    fedora)
      pkgs=(extra-cmake-modules
              cmake
              ninja-build
              gcc-c++
              qt6-qtbase-devel
              kf6-ki18n-devel
              kf6-kconfig-devel
              kf6-krunner-devel
              kf6-kio-devel
              kf6-knotifications-devel)
      check_fn=check_fedora
      install_cmd="sudo dnf install"
      ;;
    *)
      case "$ID_LIKE" in
          *arch*)
            check_fn=check_arch
            install_cmd="sudo pacman -S --needed"
            ;;
          *debian*)
            check_fn=check_debian
            install_cmd="sudo apt-get install -y"
            ;;
          *)
            echo "Unsupported distro: $ID"
            exit 1
            ;;
      esac
      ;;
esac

"$check_fn"

if [ ${#missing[@]} -eq 0 ]; then
    echo "All dependencies are installed."
else
    echo "Missing packages: ${missing[*]}"
    echo "Install them now? [y/N] "
    read -r answer
    if [ "$answer" = "y" ] || [ "$answer" = "Y" ]; then
        $install_cmd "${missing[@]}"
    fi
fi

if [ ! -d build ]
then
    cmake -B build -DCMAKE_BUILD_TYPE=Release
fi
cmake --build build
sudo cmake --install build

kbuildsycoca6 --noincremental
kquitapp6 krunner 2>/dev/null

echo "Installed successfully."