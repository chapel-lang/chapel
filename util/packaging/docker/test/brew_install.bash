#!/usr/bin/env bash

Script used in docker exec command to test homebrew formula
brew test-bot --only-tap-syntax
if [ $? -ne 0 ]; then
      echo "brew test-bot --only-tap-syntax failed" 
      exit 1
else
      echo "brew test-bot --only-tap-syntax succeeded"
fi

brew-test --only-cleanup-before
if [ $? -ne 0 ]; then
      echo "brew-test --only-cleanup-before failed" 
      exit 1
else
      echo "brew-test --only-cleanup-before succeeded"
fi

brew test-bot --only-setup
if [ $? -ne 0 ]; then
      echo "brew test-bot --only-setup failed" 
      exit 1
else
      echo "brew test-bot --only-setup succeeded"
fi

brew test-bot --only-formulae --junit --only-json-tab --skip-dependents --testing-formula=chapel --added-formulae= --deleted-formulae=
if [ $? -ne 0 ]; then
      echo "brew test-bot --only-formulae --junit --only-json-tab --skip-dependents --testing-formula=chapel --added-formulae= --deleted-formulae= failed" 
      exit 1
else
      echo "brew test-bot --only-formulae --junit --only-json-tab --skip-dependents --testing-formula=chapel --added-formulae= --deleted-formulae= succeeded"
fi

brew test-bot --only-formulae-dependents --junit --testing-formulae=chapel --skipped-or-failed-formulae=chapel
if [ $? -ne 0 ]; then
      echo "brew test-bot --only-formulae-dependents --junit --testing-formulae=chapel --skipped-or-failed-formulae=chapel failed" 
      exit 1
else
      echo "brew test-bot --only-formulae-dependents --junit --testing-formulae=chapel --skipped-or-failed-formulae=chapel succeeded"
fi
