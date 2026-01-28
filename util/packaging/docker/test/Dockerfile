# This is a container used to run homebrew-ci

# Get the homebrew ubuntu container
FROM ghcr.io/homebrew/ubuntu22.04:main

# Redundant to brew_install.bash but imporant for anyone working in this container
# which can be ran without brew_install.bash
ENV HOMEBREW_NO_AUTO_UPDATE=1
ENV HOMEBREW_NO_INSTALL_FROM_API=1
ENV HOMEBREW_DEVELOPER=1

RUN mkdir -p /home/linuxbrew
# only really needed for interactive debugging
# RUN sudo apt-get update && sudo apt-get install -y vim
# ENV EDITOR=vim

# Do the updates before applying our chapel.rb changes
# Update Homebrew itself
RUN brew update
# Upgrade to the latest version of all installed formulae
RUN brew upgrade

# necessary so that updated chapel.rb formula can be committed, which is only way it
# will be tested
RUN git config --global user.name "Chapel Tester"
RUN git config --global user.email "chapeluser@nomail.com"

# COPY chapel.rb and chapel*.tar.gz inside the container to run homebrew install
COPY chapel.rb /home/linuxbrew/.linuxbrew/Homebrew/Library/Taps/homebrew/homebrew-core/Formula/c/chapel.rb
COPY chapel*.tar.gz /home/linuxbrew/
WORKDIR /home/linuxbrew/.linuxbrew/Homebrew/Library/Taps/homebrew/homebrew-core/

# Hack: Insert bogus bottle information into the formula, which is required for
# `brew test-bot` to not ignore install failures (see
# https://github.com/Homebrew/homebrew-test-bot/issues/791).
COPY brew_get_bogus_bottles.bash brew_get_bogus_bottles.bash
RUN brew_get_bogus_bottles.bash | sed -i -e '/<bottle-block-placeholder-injected-during-testing>/r /dev/stdin' -e '/<bottle-block-placeholder-injected-during-testing>/d' Formula/c/chapel.rb
RUN rm brew_get_bogus_bottles.bash

# must commit the change or brew doctor will complain
RUN git add Formula/c/chapel.rb && git commit -m "update chapel.rb for nightly testing"
WORKDIR /home/linuxbrew/
