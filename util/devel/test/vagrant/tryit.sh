#!/bin/bash

# Runs a command (or series of commands) on all the VMs.
# Launches each VM, runs the command, and then shuts it down.
# Prints a summary of the output at the end.
# Saves all the command output in the file called log.

LOG=log

declare -a NAME
declare -a RESULT

echo > log
echo > log2
echo "Running command on images:" | tee -a log
echo "$*" | tee -a log

DIR=`pwd`

i=0

# compute longest length for padding
maxlen=0
for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    if (( ${#name} > $maxlen ))
    then
      maxlen=${#name}
    fi
  fi
done

for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    cd $name
    display=`printf "%${maxlen}s" $name`
    NAME[$i]=$display
    echo
    echo "     ---- $display ---- " | tee -a "$DIR"/log
    vagrant up 2>&1 | tee -a "$DIR"/log
    ( vagrant ssh --command "$*" -- -q 2>&1 && echo >&3 ok || echo >&3 error exit $? ) 3> "$DIR"/log2 | tee -a "$DIR"/log
    case "$(tail -n 1 "$DIR"/log2)" in
    ( ok )
      echo
      lastline=`tail -n 1 "$DIR"/log`
      RESULT[$i]="  OK: $lastline"
      ;;
    ( error* )
      lastline=`tail -n 1 "$DIR"/log`
      RESULT[$i]="FAIL: $lastline"
      echo "     FAIL:" $name
      echo "     Leaving virtual machine up"
      cd ..
      echo "     You might want to run:"
      echo
      echo "     cd" $name
      echo "     vagrant ssh"
      echo "     " $*
      echo
      echo
      exit 1
      ;;
    esac
    vagrant halt 2>&1 | tee -a "$DIR"/log
    cd "$DIR"

    ((i++))
  fi
done

echo "     -------- SUMMARY --------"

i=0
for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    echo "${NAME[$i]}:${RESULT[$i]}"

    ((i++))
  fi
done
