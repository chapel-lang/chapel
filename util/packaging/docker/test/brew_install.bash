#!/usr/bin/env bash
# Important to set HOMEBREW_NO_INSTALL_FROM_API to actually test changes
export HOMEBREW_NO_INSTALL_FROM_API=1
# Might not be needed, but also doesn't hurt and matches homebrew CI
export HOMEBREW_NO_AUTO_UPDATE=1

#Script used in docker exec command to test homebrew formula
# brew test-bot --only-tap-syntax
# if [ $? -ne 0 ]; then
#       echo "brew test-bot --only-tap-syntax failed"
#       exit 1
# else
#       echo "brew test-bot --only-tap-syntax succeeded"
# fi

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

# This is the bulk of the testing and the same command that Homebrew CI runs
brew test-bot  --skip-online-checks --only-formulae --junit --only-json-tab --skip-dependents --testing-formulae="chapel" --added-formulae="" --deleted-formulae=""
if [ $? -ne 0 ]; then
      echo "brew test-bot --skip-online-checks --only-formulae --junit --only-json-tab --skip-dependents --testing-formulae="chapel" --added-formulae="" --deleted-formulae="" failed"
      exit 1
else
      echo "brew test-bot --skip-online-checks --only-formulae --junit --only-json-tab --skip-dependents --testing-formulae="chapel" --added-formulae="" --deleted-formulae="" succeeded"
fi
