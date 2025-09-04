#!/bin/bash

# Note, this is not run all as root, so use sudo where needed

sudo apt-get update # hide

# homebrew recommends installing this
sudo apt-get install -y build-essential # hide
# many of these are in build-essential
sudo apt-get install -y gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config # hide

export NONINTERACTIVE=1 # hide

# install homebrew
NONINTERACTIVE=1 /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)" #hide

# make sure homebrew commands are available in future logins
echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"' >> /home/vagrant/.bashrc #hide
echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"' >> /home/vagrant/.bash_profile #hide

# and right now
eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)" #hide

# recommended by homebrew
brew install gcc #hide

# install some dependencies in homebrew
brew install cmake python gmp llvm #unsudo

# install LLVM version 20 as we don't support 21 yet
brew install llvm@20 #unsudo

# we could use Homebrew's gcc if that becomes important in the future:
# # link the homebrew-installed gcc-* to gcc
# cd /home/linuxbrew/.linuxbrew/bin/
# GCC_VER=`ls g++-* | cut -d - -f 2`
# ln -s gcc-$GCC_VER gcc
# ln -s g++-$GCC_VER g++

  # /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
  # echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"' >> /home/vagrant/.profile
  # eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"
  # sudo apt-get install build-essential
  # brew install gcc
  # brew install llvm@11
  # brew install llvm  # useful for finding errors in order

