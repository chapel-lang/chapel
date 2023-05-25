#!/usr/bin/env bash

cd chapel && GIT_PAGER=cat git log --oneline -n 1 && GIT_PAGER=cat git diff
