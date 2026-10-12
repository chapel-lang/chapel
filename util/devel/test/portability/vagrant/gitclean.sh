#!/bin/bash

# git clean in VM's chapel directory

set -exuo pipefail

./tryit.sh "cd chapel && GIT_PAGER=cat git clean -fdx"
