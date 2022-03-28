#!/usr/bin/env bash

# Update the Chapel git repository in each VM to main

./tryit.sh "cd chapel && git fetch origin --depth=1 && git checkout origin/main && GIT_PAGER=cat git log --oneline -n 1 && GIT_PAGER=cat git diff"

