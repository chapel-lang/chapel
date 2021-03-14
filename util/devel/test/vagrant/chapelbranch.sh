#!/usr/bin/env bash

# check out a particular branch in the VMs chapel directory
#
# Usage:
#   ./chapelbranch.sh <github-user-name> <github-branch-name>
#

GITHUB_USER=$1
GITHUB_BRANCH=$2

if [ -z "$GITHUB_USER" -o -z "$GITHUB_BRANCH" ]
then
  echo "Usage: $0 <github-user-name> <github-branch>"
fi

echo "Checking out github branch $GITHUB_USER/$GITHUB_BRANCH"

./tryit.sh "cd chapel && git remote add $GITHUB_USER https://github.com/$GITHUB_USER/chapel && git fetch $GITHUB_USER $GITHUB_BRANCH && git reset --hard $GITHUB_USER/$GITHUB_BRANCH"

