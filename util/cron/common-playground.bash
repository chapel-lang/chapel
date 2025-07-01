#!/usr/bin/env bash

function checkout_branch()
{
  local user=$1
  local branch=$2
  # log_info relies on common.bash being sourced first, not enforced by this file
  log_info "Checking out branch $branch from user $user"
  git branch -D $user-$branch || true
  git checkout -b $user-$branch
  git pull --no-rebase https://github.com/$user/chapel.git $branch
}
