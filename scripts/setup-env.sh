#!/usr/bin/env bash

ENTT_VERSION_TAG="${ENTT_VERSION_TAG:-v3.9.0}";
OGRE_VERSION_TAG="${OGRE_VERSION_TAG:-v13.3.1}";
SDL2_VERSION_TAG="${SDL2_VERSION_TAG:-release-2.0.20}";
BOOST_VERSION_TAG="${BOOST_VERSION_TAG:-boost-1.78.0}";

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
  # We could be using guard clauses
else

  # Install dependencies if is needed
  echo "===[ Checking Dependencies ]==========================================="
  DEPENDENCIES=(libgbm-dev libgles2-mesa-dev libxt-dev libxaw7-dev doxygen zziplib-bin cmake-qt-gui libassimp-dev libassimp5 assimp-utils libopenexr25 libopenexr-dev libbullet-dev libbullet-extras-dev openexr cmake)
  for i in "${DEPENDENCIES[@]}"; do
    if [ "$(is_package_installed $i)" = "no" ]; then
      echo $i
      sudo apt install -y $i
    else
      echo "Found $i"
    fi
  done

####################################### EnTT START #################################################

# Set EnTT version
  
  echo "======================================================================="
  echo "Building and installing EnTT $ENTT_VERSION_TAG"
  echo "-----------------------------------------------------------------------"

  cd third_party/entt

  echo "===[ Checkout $ENTT_VERSION_TAG ]======================================"
  git checkout $ENTT_VERSION_TAG

  echo "===[ Copy EnTT src ]==================================================="
  # Install entt
  mkdir ../../lib
  mkdir ../../lib/entt
  cp ./src/entt/ ../../lib -r

  cd ../../
  
  echo "===[ EnTT Done ]======================================================="

######################################### EnTT END #################################################

fi