#!/usr/bin/env bash

GITHUB_USER=$1
GITHUB_BRANCH=$2

if [ -z "$GITHUB_USER" -o -z "$GITHUB_BRANCH" ]
then
  echo "Usage: $0 <github-user-name> <github-branch>"
fi

echo "Checking out github branch $GITHUB_USER/$GITHUB_BRANCH"

if [ ! -d chapel ]
then
  echo could not find chapel directory, exiting
  exit 1
fi

cd chapel
git remote add $GITHUB_USER https://github.com/$GITHUB_USER/chapel

git fetch --depth=1 $GITHUB_USER $GITHUB_BRANCH && git reset --hard FETCH_HEAD
