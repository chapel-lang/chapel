#!/usr/bin/env bash

function checkout_branch()
{
  local user=$1
  local branch=$2
  git branch -D $user-$branch
  git checkout -b $user-$branch
  git pull https://github.com/$user/chapel.git $branch
}
