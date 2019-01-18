#!/bin/bash

# git clean in each VM's chapel directory

./tryit.sh "cd chapel && GIT_PAGER=cat git clean -fdx"
