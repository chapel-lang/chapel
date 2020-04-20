#!/bin/bash

STAGE=""
STAGE_SET=0
PREFIX=""
# Different from DESTDIR, which is for staged installs
# this variable is for installing the Chapel directory in one place
# (to mirror release / source checkout)
DEST_DIR=""

# Argument parsing
for arg in "$@"
do
  case $arg in
    #e.g. -s|--short)
    --stage=*)
      STAGE="${arg#*=}"
      STAGE_SET=1
      shift
      ;;
    *)
      echo
      echo "Usage: $0 [--stage=DESTDIR]"
      echo
      echo "       --stage=DESTDIR prepends DESTDIR to prefix"
      echo "                e.g. for staged installation as with"
      echo "                     the DESTDIR Makefile variable"
      echo
      exit -1
    ;;
  esac

done


if [ ! -d "$CHPL_HOME" ]
then
  if [ -d "util/chplenv" ]
  then
    export CHPL_HOME="$PWD"
    echo "Detecting CHPL_HOME as $CHPL_HOME"
  else
    echo Exiting: CHPL_HOME not set or not a directory
    exit -1
  fi
fi

if [ -f "$CHPL_HOME/configured-prefix" ]
then
  read -r PREFIX < "$CHPL_HOME/configured-prefix"
fi

if [ ! -z "$PREFIX" ]
then
  PREFIX="${STAGE}${PREFIX}"
  if [ "$STAGE_SET" -ne 0 ]
  then
    mkdir -p "$PREFIX"
  fi
  if [ ! -d "$PREFIX" ]
  then
    echo "Exiting: Installation prefix path '$PREFIX' does not exist"
    exit -1
  fi
else
  if [ -f "$CHPL_HOME/configured-chpl-home" ]
  then
    read -r DEST_DIR < "$CHPL_HOME/configured-chpl-home"
    DEST_DIR="${STAGE}${DEST_DIR}"
    if [ "$STAGE_SET" -ne 0 ]
    then
      mkdir -p "$DEST_DIR"
    fi
    if [ ! -d "$DEST_DIR" ]
    then
      echo "Exiting: Installation dest path '$DEST_DIR' does not exist"
      exit -1
    fi
  else
    echo "Exiting: Installation path not configured."
    echo "         See ./configure --help"
    exit -1
  fi
fi

case "$PREFIX" in
  '""')
    echo Exiting: PREFIX was not set.
    echo          ... did you forget to run configure ?
    exit -1
    ;;
  *\\*)
    # For now, fail if PREFIX contains a backslash.
    # The issue is that the C string and Bash variable might treat
    # the prefix differently.
    echo Exiting: PREFIX contains backslash
    echo          ... please use a prefix that does not contain backslash
    exit -1
    ;;
  *)
    ;;
esac

# Gather the version number for the Chapel binary.
CHPL_BIN_SUBDIR=`"$CHPL_HOME"/util/chplenv/chpl_bin_subdir.py`
VERS=`$CHPL_HOME/bin/$CHPL_BIN_SUBDIR/chpl --version`
# Remove the "chpl version " part
VERS=${VERS#chpl version }
# Replace the periods with spaces.
VERS=${VERS//./ }
VERS_ARRAY=($VERS)
MAJOR_VERSION=${VERS_ARRAY[0]}
MINOR_VERSION=${VERS_ARRAY[1]}
VERS="${MAJOR_VERSION}.${MINOR_VERSION}"
echo "Installing Chapel version $VERS"

if [ ! -z "$PREFIX" ]
then
  DEST_RUNTIME_LIB="$PREFIX/lib/chapel/$VERS/runtime/lib"
  DEST_RUNTIME_INCL="$PREFIX/lib/chapel/$VERS/runtime/include"
  DEST_THIRD_PARTY="$PREFIX/lib/chapel/$VERS/third-party"
  DEST_CHPL_HOME="$PREFIX/share/chapel/$VERS"
  echo "Installing Chapel split to bin, lib, share to $PREFIX"
  if [ "$CHPL_HOME" = "$PREFIX" ]
  then
    echo
    echo "Error: destination prefix is Chapel source directory"
    echo "Please run configure again to select a different installation path"
    exit -1
  fi
else
  DEST_RUNTIME_LIB="$DEST_DIR/lib"
  DEST_RUNTIME_INCL="$DEST_DIR/runtime/include"
  DEST_THIRD_PARTY="$DEST_DIR/third-party"
  DEST_CHPL_HOME="$DEST_DIR"
  echo "Installing Chapel-as-a-directory to $DEST_DIR"
  if [ "$CHPL_HOME" = "$DEST_DIR" ]
  then
    echo
    echo "Error: destination directory is Chapel source directory"
    echo "Please run configure again to select a different installation path"
    exit -1
  fi
fi

echo "  Installing runtime lib to      $DEST_RUNTIME_LIB"
echo "  Installing runtime include to  $DEST_RUNTIME_INCL"
echo "  Installing third-party to      $DEST_THIRD_PARTY"
echo "  Installing modules, scripts to $DEST_CHPL_HOME"

myinstalldir () {
  FROM="$CHPL_HOME/$1"
  TO="$2"
  #echo myinstalldir $FROM $TO
  mkdir -p "$TO"

  if [ ! -d "$FROM" ]
  then
    echo "Error: source directory '$FROM' missing"
    exit -1
  fi
  if [ ! -d "$TO" ]
  then
    echo "Error: could not find/create destination directory '$TO'"
    exit -1
  fi

  ( cd "$FROM" ; tar cf - . ) | ( cd "$TO" ; tar xf - )

  if [ $? -ne 0 ]
  then
    echo "Error: failed directory copy '$FROM' '$TO'"
    exit -1
  fi
}

myinstallfile () {
  FROM="$CHPL_HOME/$1"
  TO="$2"

  mkdir -p "$TO"

  if [ ! -f "$FROM" ]
  then
    echo "Error: source file '$FROM' missing"
    exit -1
  fi
  if [ ! -d "$TO" ]
  then
    echo "Error: could not find/create destination directory '$TO'"
    exit -1
  fi

  #echo myinstallfile $FROM $TO
  cp "$FROM" "$TO"

  if [ $? -ne 0 ]
  then
    echo "Error: failed cp '$FROM' '$TO'"
    exit -1
  fi
}

myinstallfileto () {
  FROM="$CHPL_HOME/$1"
  TO="$2"
  #echo myinstallfile $FROM $TO
  if [ ! -f "$FROM" ]
  then
    echo "Error: source file '$FROM' missing"
    exit -1
  fi

  cp "$FROM" "$TO"

  if [ $? -ne 0 ]
  then
    echo "Error: failed cp '$FROM' '$TO'"
    exit -1
  fi

  if [ ! -f "$TO" ]
  then
    echo "Error: could not install to '$TO'"
    exit -1
  fi
}


# copy chpl
if [ ! -z "$PREFIX" ]
then
  # TODO -- handle chpldoc
  #   these are symbol links to chpl
  myinstallfile "bin/$CHPL_BIN_SUBDIR"/chpl "$PREFIX/bin"
else
  tmp_bin_dir="bin/$CHPL_BIN_SUBDIR"
  myinstallfile "$tmp_bin_dir"/chpl "$DEST_DIR/$tmp_bin_dir"
fi

# copy runtime lib
myinstalldir  lib                     "$DEST_RUNTIME_LIB"

# copy runtime include
myinstalldir  runtime/include         "$DEST_RUNTIME_INCL"

# copy README etc
myinstallfile ACKNOWLEDGEMENTS.md     "$DEST_CHPL_HOME"
myinstallfile CHANGES.md              "$DEST_CHPL_HOME"
myinstallfile CONTRIBUTORS.md         "$DEST_CHPL_HOME"
myinstallfile COPYRIGHT               "$DEST_CHPL_HOME"
myinstallfile GOALS.md                "$DEST_CHPL_HOME"
myinstallfile LICENSE                 "$DEST_CHPL_HOME"
myinstallfile LICENSE.chapel          "$DEST_CHPL_HOME"
myinstallfile README.rst              "$DEST_CHPL_HOME"

# copy modules
myinstalldir  modules                 "$DEST_CHPL_HOME"/modules

# copy util/printchplenv
myinstallfile util/printchplenv       "$DEST_CHPL_HOME"/util/

# copy util/chplenv
myinstalldir  util/chplenv            "$DEST_CHPL_HOME"/util/chplenv/

# copy util/config/compileline
# (needed for LLVM builds)
myinstallfile util/config/compileline "$DEST_CHPL_HOME"/util/config/

# copy util/config/fixpath.py
# (needed by setchplenv*)
myinstallfile util/config/fixpath.py  "$DEST_CHPL_HOME"/util/config/

# copy util/config/replace-paths.py
# (needed by --library --library-makefile compilations)
myinstallfile util/config/replace-paths.py  "$DEST_CHPL_HOME"/util/config/


if [ ! -z "$DEST_DIR" ]
then
  # copy util/setchplenv*, util/quickstart
  for f in util/setchplenv*
  do
    myinstallfile "$f"                "$DEST_CHPL_HOME"/util/
  done

  myinstalldir  util/quickstart       "$DEST_CHPL_HOME"/util/quickstart/
fi

# copy compiler/etc
myinstalldir  compiler/etc            "$DEST_CHPL_HOME"/compiler/etc/

# copy runtime/etc
myinstalldir  runtime/etc             "$DEST_CHPL_HOME"/runtime/etc/

# copy runtime/make
myinstalldir  runtime/make            "$DEST_CHPL_HOME"/runtime/make/

# copy make
# TODO - just Makefile.base, platform, tasks ?
myinstalldir  make                    "$DEST_CHPL_HOME"/make/

# copy third-party makefile snippets
cd third-party
THIRD_PARTY_DIRS=`echo *`
cd ..

for dir in $THIRD_PARTY_DIRS
do
  #echo "Considering 3p dir $dir"
  if [ -f third-party/"$dir"/Makefile.include ]
  then
    for f in third-party/"$dir"/Makefile*
    do
      myinstallfile "$f"  "$DEST_THIRD_PARTY"/"$dir"
    done
  fi
  if [ -d third-party/"$dir"/install ]
  then
    myinstalldir "third-party/$dir/install" "$DEST_THIRD_PARTY/$dir/install/"
  fi
done

# copy filter-llvm-config.awk
myinstallfile third-party/llvm/filter-llvm-config.awk "$DEST_THIRD_PARTY"/llvm

# copy find-llvm-config.sh
myinstallfile third-party/llvm/find-llvm-config.sh "$DEST_THIRD_PARTY"/llvm

# copy LLVM_VERSION
myinstallfile third-party/llvm/LLVM_VERSION "$DEST_THIRD_PARTY"/llvm

# copy utf8-decoder header
myinstallfile third-party/utf8-decoder/utf8-decoder.h "$DEST_THIRD_PARTY"/utf8-decoder/

# copy mason
if [ -f tools/mason/mason ]
then
  if [ ! -z "$PREFIX" ]
  then
    myinstallfile tools/mason/mason "$PREFIX/bin"
  else
    myinstallfile tools/mason/mason "$DEST_CHPL_HOME/tools/mason"
    ln -s "$DEST_CHPL_HOME/tools/mason/mason" "$DEST_DIR/bin/$CHPL_BIN_SUBDIR"/mason
  fi
fi

# copy chplconfig
if [ -f chplconfig ]
then
  if [ ! -z "$PREFIX" ]
  then
    myinstallfileto chplconfig "$PREFIX/lib/chapel/$VERS/chplconfig"
  else
    myinstallfileto chplconfig "$DEST_CHPL_HOME/chplconfig"
  fi
fi

# Clean up: remove any .pyc files
find "$DEST_CHPL_HOME" -name '*.pyc' -delete
# Clean up: remove any .timestamp files
find "$DEST_RUNTIME_LIB" -name '.timestamp' -delete
