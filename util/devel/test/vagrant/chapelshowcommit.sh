#!/usr/bin/env bash

# Show the current commit of each VM's chapel git repository

./tryit.sh "cd chapel && GIT_PAGER=cat git log --oneline -n 1 && GIT_PAGER=cat git diff"

