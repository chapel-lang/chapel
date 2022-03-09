#!/bin/bash

# homebrew recommends installing this
apt-get install -y build-essential
# many of these are in build-essential
apt-get install -y gcc g++ m4 perl python3 python3-pip python3-venv python3-dev bash make mawk git pkg-config cmake

export NONINTERACTIVE=1

# install homebrew
NONINTERACTIVE=1 /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# make sure homebrew commands are available in future logins
echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"' >> /home/vagrant/.bashrc

# and right now
eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"

# recommended by homebrew
brew install gcc

# install some dependencies in homebrew
brew install python gmp llvm@12


  # /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
  # echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"' >> /home/vagrant/.profile
  # eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"
  # sudo apt-get install build-essential
  # brew install gcc
  # brew install llvm@11
  # brew install llvm  # useful for finding errors in order

