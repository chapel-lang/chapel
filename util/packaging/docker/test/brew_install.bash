#!/usr/bin/env bash

set -ex

# Hack to inject bogus bottle block into chapel.rb for testing purposes.
cd /home/linuxbrew/.linuxbrew/Homebrew/Library/Taps/homebrew/homebrew-core
/home/linuxbrew/brew_get_bogus_bottles.bash | sed -i -e '/<bottle-block-placeholder-injected-during-testing>/r /dev/stdin' -e '/<bottle-block-placeholder-injected-during-testing>/d' Formula/c/chapel.rb
git add Formula/c/chapel.rb && git commit -m "update chapel.rb for nightly testing" && PAGER=cat git show
cd /home/linuxbrew

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

brew test-bot --only-setup

brew test-bot --only-formulae-dependents --junit --testing-formulae=chapel --skipped-or-failed-formulae=chapel

# This is the bulk of the testing and the same command that Homebrew CI runs
brew test-bot  --skip-online-checks --only-formulae --junit --only-json-tab --skip-dependents --testing-formulae="chapel" --added-formulae="" --deleted-formulae="" \
  | awk 'tolower($0)~/failed steps? ignored/{r=1} 1; END{exit(r)}'
