#!/bin/bash

# Runs a command (or series of commands) on all the VMs.
# Launches each VM, runs the command, and then shuts it down.
# Prints a summary of the output at the end.
# Saves all the command output in the file called log.

LOG=log

declare -a NAME
declare -a RESULT

echo > log
echo "Running command on images:" | tee -a log
echo "$*" | tee -a log

i=0

# compute longest length for padding
maxlen=0
for name in *
do
  if [ -f $name/Vagrantfile ]
  then
    if (( ${#name} > $maxlen ))
    then
      maxlen=${#name}
    fi
  fi
done

for name in *
do
  if [ -f $name/Vagrantfile ]
  then
    cd $name
    display=`printf "%${maxlen}s" $name`
    NAME[$i]=$display
    echo
    echo "     ---- $display ---- " | tee -a ../log
    vagrant up 2>&1 | tee -a ../log
    vagrant ssh --command "$*" -- -q 2>&1 | tee -a ../log
    if (( $? == 0 ))
    then
      echo
      lastline=`tail -n 1 ../log`
      RESULT[$i]="  OK: $lastline"
      #echo "     SUCCESS:" $name
    else
      lastline=`tail -n 1 ../log`
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
    fi
    vagrant halt 2>&1 | tee -a ../log
    cd ..

    ((i++))
  fi
done

echo "     -------- SUMMARY --------"

i=0
for name in *
do
  if [ -f $name/Vagrantfile ]
  then
    echo "${NAME[$i]}:${RESULT[$i]}"

    ((i++))
  fi
done
