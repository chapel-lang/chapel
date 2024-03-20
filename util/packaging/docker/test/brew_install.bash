#!/usr/bin/env bash
export HOMEBREW_SIMULATE_MACOS_ON_LINUX=1
#Update homebrew
brew update 

#Script used in docker exec command to test homebrew formula
brew test-bot --only-tap-syntax
if [ $? -ne 0 ]; then
      echo "brew test-bot --only-tap-syntax failed" 
      exit 1
else
      echo "brew test-bot --only-tap-syntax succeeded"
fi

# tests commands
brew test-bot --only-cleanup-before
if [ $? -ne 0 ]; then
      echo "brew test-bot --only-cleanup-before failed" 
      exit 1
else
      echo "brew test-bot --only-cleanup-before succeeded"
fi

brew test-bot --only-setup
    if [ $? -ne 0 ]; then
      echo "brew test-bot --only-setup failed" 
      exit 1
      else
      echo "brew test-bot --only-setup succeeded"
    fi

brew test-bot --only-formulae-dependents --junit --testing-formulae=chapel --skipped-or-failed-formulae=chapel
if [ $? -ne 0 ]; then
      echo "brew test-bot --only-formulae-dependents --junit --testing-formulae=chapel --skipped-or-failed-formulae=chapel failed" 
      exit 1
else
      echo "brew test-bot --only-formulae-dependents --junit --testing-formulae=chapel --skipped-or-failed-formulae=chapel succeeded"
fi
