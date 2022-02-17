#!/usr/bin/env bash

HELP=$(cat <<-END
Usage: ./setup.sh
Setup engine build environment.
.
END
)

is_package_installed() {
  PACKAGE=$1;
  IS_INSTALL=$(dpkg-query -W --showformat='${Status}\n' $PACKAGE | grep "install ok installed")
  echo $(if [ "$IS_INSTALL" = "install ok installed" ]; then echo "yes"; else echo "no"; fi)
}

if [ "$1" = "-h" ] || [ "$1" = "help" ]; then
  echo "$HELP"
else
  # Set ogre version
  OGRE_VERSION_TAG="${OGRE_VERSION_TAG:-v13.3.0}";
  echo "======================================================================="
  echo "Building and installing Ogre $OGRE_VERSION_TAG"
  echo "-----------------------------------------------------------------------"

  # Install dependencies if is needed
  echo "===[ Checking Dependencies ]==========================================="
  DEPENDENCIES=(libgles2-mesa-dev libsdl2-dev libxt-dev libxaw7-dev doxygen zziplib-bin cmake-qt-gui libassimp-dev libassimp5 assimp-utils libopenexr25 libopenexr-dev libbullet-dev libbullet-extras-dev libboost-all-dev libimgui-dev openexr cmake)
  for i in "${DEPENDENCIES[@]}"; do
    if [ "$(is_package_installed $i)" = "no" ]; then
      echo $i
      sudo apt install -y $i
    else
      echo "Found $i"
    fi
  done

  echo "===[ Cloning Ogre ]===================================================="
  git clone https://github.com/OGRECave/ogre.git
  cd ogre

  echo "===[ Checkout $OGRE_VERSION_TAG ]======================================"
  git checkout $OGRE_VERSION_TAG

  echo "===[ Build ]==========================================================="
  # Build ogre
  cmake .
  cmake --build . --config Release

  echo "===[ Install ]========================================================="
  # Install ogre
  sudo cmake --build . --config Release --target install

  echo "===[ Clean Up ]========================================================"
  # Remove ogre repo
  cd ..
  rm -rf ogre
  
  echo "===[ Done ]============================================================"

fi