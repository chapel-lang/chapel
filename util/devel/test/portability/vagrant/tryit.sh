#!/bin/bash

# Runs a command (or series of commands) on a VM.
# Launches the VM, runs the command, and then shuts it down.
# Prints a summary of the output at the end.
# Saves all the command output in the file called log.

# Expects the VM name to be given as an environment variable. The special name
# "all" can be used to run the command on all VMs.

if [ -z "$VM_NAME" ]
then
  echo "Please set the VM_NAME environment variable to the name of the VM you want to run this on."
  exit 1
fi

LOG=log

declare -a NAME
declare -a RESULT

# enable resizing for FreeBSD images
export VAGRANT_EXPERIMENTAL="disks"

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

exit_status=0
for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    # skip if this isn't a desired VM to run on
    if [ "$VM_NAME" != "all" ]
    then
      if [ "$name" != "current/$VM_NAME" ]
      then
        continue
      fi
    fi
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
      echo "     You might want to run:"
      echo
      echo "     cd" $name
      echo "     vagrant ssh"
      echo "     " $*
      echo
      echo
      exit_status=1
      #uncomment the below line to make errors fatal
      #exit 1
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

exit $exit_status
