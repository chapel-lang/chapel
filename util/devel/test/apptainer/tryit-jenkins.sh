#!/bin/bash

# Runs a command (or series of commands) on all the images.
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
  if [ -f $name/image.def ]
  then
    if (( ${#name} > $maxlen ))
    then
      maxlen=${#name}
    fi
  fi
done

for name in current/*
do
  if [ -f $name/image.def ]
  then
    cd $name
    display=`printf "%${maxlen}s" $name`
    NAME[$i]=$display
    echo
    echo "     ---- $display ---- " | tee -a "$DIR"/log
    if [ ! -e image.sif ]
    then
      echo $name/image.sif not present so building image
      ( apptainer build --fakeroot image.sif image.def 2>&1 && echo >&3 ok || echo >&3 error exit $? ) 3> "$DIR"/log2 | tee -a "$DIR"/log
    fi
    ( apptainer exec image.sif "$@" 2>&1 && echo >&3 ok || echo >&3 error exit $? ) 3> "$DIR"/log2 | tee -a "$DIR"/log
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
      echo "     You might want to run:"
      echo
      echo "     cd" $name
      echo "     apptainer shell image.sif"
      echo "     " $*
      echo
      echo
      #uncomment the below line to make errors fatal
      #exit 1
      ;;
    esac
    rm -Rf chapel
    rm -Rf image.sif
    cd "$DIR"
   
    ((i++))
  fi
done

# log2 is no longer needed so remove it
rm log2

echo "     -------- SUMMARY --------"

i=0
for name in current/*
do
  if [ -f $name/image.def ]
  then
    echo "${NAME[$i]}:${RESULT[$i]}"

    ((i++))
  fi
done
