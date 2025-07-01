#!/usr/bin/env bash

for try in 1 2 3 4 5
do

  if [ -d chapel ]
  then
    echo chapel directory already exists - updating
    cd chapel && git checkout main && git pull --ff-only && cd .. && echo UPDATED
  else
    echo cloning chapel
    git clone --reference-if-able "${REPO_CACHE_PATH:-/missing}/chapel.git" --depth 1 https://github.com/chapel-lang/chapel && echo CLONED
  fi

  if [ $? -eq 0 ]
  then
    # OK, all done, exit with success
    exit 0
  else
    echo "Error with git. Pausing for 30s."
    sleep 30
    if [ -d chapel ]
    then
      echo "Deleting the chapel directory that failed update"
      rm -rf chapel
    fi
  fi

done

# if we got here, there was an error
exit 1
