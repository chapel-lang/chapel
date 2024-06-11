#!/bin/bash

STAGE=""
STAGE_SET=0
PREFIX=""
# Different from DESTDIR, which is for staged installs
# this variable is for installing the Chapel directory in one place
# (to mirror release / source checkout)
DEST_DIR=""

# set MAKE=make if it is not set yet
: "${MAKE:=make}"

# Argument parsing
for arg in "$@"
do
  case $arg in
    #e.g. -s|--short)
    --stage=*)
      STAGE="${arg#*=}"
      if [ ! -z $STAGE ]
      then
        STAGE_SET=1
      fi
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
  mkdir -p "$PREFIX"
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
    mkdir -p "$DEST_DIR"

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
CHPL_PYTHON=`"$CHPL_HOME"/util/config/find-python.sh`
CHPL_BIN_SUBDIR=`"$CHPL_PYTHON" "$CHPL_HOME"/util/chplenv/chpl_bin_subdir.py`
VERS=`$CHPL_HOME/bin/$CHPL_BIN_SUBDIR/chpl --version`
if [ $? -ne 0 ]
then
  echo "Error: failed to run chpl --version; Have you already built the compiler using make?"
  echo "       If not, please run 'make' before running this script."
  exit -1
fi
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

  # If we wanted to exclude the built Python files, we would use e.g.
  #   tar --exclude='__pycache__' -cf -
  # However in this install script we probably want to make sure that
  # these are compiled ahead-of-time (and not with elevated
  # privileges so not in this script).
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

# this makefile target runs 'cmake' to install the compiler library, 'chpl',
# and optionally 'chpldoc' if it was built
(cd compiler && "$MAKE" install-chpl-chpldoc)


# copy compiler and runtime lib
myinstalldir  lib                     "$DEST_RUNTIME_LIB"

# copy runtime include
myinstalldir  runtime/include         "$DEST_RUNTIME_INCL"

# copy README etc
myinstallfile ACKNOWLEDGEMENTS.md     "$DEST_CHPL_HOME"
myinstallfile CHANGES.md              "$DEST_CHPL_HOME"
myinstallfile CONTRIBUTORS.md         "$DEST_CHPL_HOME"
myinstallfile COPYRIGHT               "$DEST_CHPL_HOME"
myinstallfile LICENSE                 "$DEST_CHPL_HOME"
myinstallfile LICENSE.chapel          "$DEST_CHPL_HOME"
myinstallfile README.rst              "$DEST_CHPL_HOME"

# copy modules
myinstalldir  modules                 "$DEST_CHPL_HOME"/modules

# copy util/printchplenv
myinstallfile util/printchplenv       "$DEST_CHPL_HOME"/util/

# copy util/chplenv
myinstalldir  util/chplenv            "$DEST_CHPL_HOME"/util/chplenv/

# copy util/config/
myinstalldir  util/config            "$DEST_CHPL_HOME"/util/config/

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
  # copy Makefiles (which are used by the C backend)
  for f in third-party/"$dir"/Makefile*
  do
    if [ -f "$f" ]
    then
      myinstallfile "$f"  "$DEST_THIRD_PARTY"/"$dir"
    fi
  done

  # copy any installed libraries
  if [ -d third-party/"$dir"/install ]
  then
    myinstalldir "third-party/$dir/install" "$DEST_THIRD_PARTY/$dir/install/"
  fi

  # chpl-venv also needs to copy chpldoc-sphinx-project
  # but this never contains executables so should go in DEST_CHPL_HOME
  if [ -d third-party/"$dir"/chpldoc-sphinx-project ]
  then
    myinstalldir "third-party/$dir/chpldoc-sphinx-project" "$DEST_CHPL_HOME/third-party/$dir/chpldoc-sphinx-project/"
  fi
done

# copy filter-llvm-config.awk
myinstallfile third-party/llvm/filter-llvm-config.awk "$DEST_THIRD_PARTY"/llvm

# copy utf8-decoder header
myinstallfile third-party/utf8-decoder/utf8-decoder.h "$DEST_THIRD_PARTY"/utf8-decoder/


MASON="bin/$CHPL_BIN_SUBDIR"/mason

# copy mason
if [ -f "$MASON" ]
then
  if [ ! -z "$PREFIX" ]
  then
    myinstallfile "$MASON" "$PREFIX/bin"
  else
    myinstallfile "$MASON" "$DEST_CHPL_HOME/tools/mason"
    ln -s "$DEST_CHPL_HOME/tools/mason/mason" "$DEST_DIR/bin/$CHPL_BIN_SUBDIR"/mason
  fi
fi

C2CHAPEL="bin/$CHPL_BIN_SUBDIR"/c2chapel

# copy c2chapel
if [ -f "$C2CHAPEL" ]
then
  myinstalldir "tools/c2chapel/install" "$DEST_CHPL_HOME/tools/c2chapel/install"
  myinstallfile "tools/c2chapel/c2chapel" "$DEST_CHPL_HOME/tools/c2chapel"
  myinstallfile "tools/c2chapel/c2chapel.py" "$DEST_CHPL_HOME/tools/c2chapel"
  myinstallfile "tools/c2chapel/utils/custom.h" "$DEST_CHPL_HOME/tools/c2chapel/util"

  if [ ! -z "$PREFIX" ]
  then
    ln -s "$DEST_CHPL_HOME/tools/c2chapel/c2chapel" "$PREFIX/bin"/c2chapel
  else
    ln -s "$DEST_CHPL_HOME/tools/c2chapel/c2chapel" "$DEST_DIR/bin/$CHPL_BIN_SUBDIR"/c2chapel
  fi
fi

CHPLCHECK="bin/$CHPL_BIN_SUBDIR"/chplcheck

# copy chplcheck
if [ -f "$CHPLCHECK" ]
then
  myinstallfile "tools/chplcheck/chplcheck" "$DEST_CHPL_HOME/tools/chplcheck"
  myinstalldir "tools/chplcheck/src" "$DEST_CHPL_HOME/tools/chplcheck/src"

  if [ ! -z "$PREFIX" ]
  then
    ln -s "$DEST_CHPL_HOME/tools/chplcheck/chplcheck" "$PREFIX/bin"/chplcheck
  else
    ln -s "$DEST_CHPL_HOME/tools/chplcheck/chplcheck" "$DEST_DIR/bin/$CHPL_BIN_SUBDIR"/chplcheck
  fi
fi

CHPL_LANGUAGE_SERVER="bin/$CHPL_BIN_SUBDIR"/chpl-language-server

# copy chpl-language-server
if [ -f "$CHPL_LANGUAGE_SERVER" ]
then
  myinstallfile "tools/chpl-language-server/chpl-language-server" "$DEST_CHPL_HOME/tools/chpl-language-server"
  myinstallfile "tools/chpl-language-server/chpl-shim" "$DEST_CHPL_HOME/tools/chpl-language-server"
  myinstalldir "tools/chpl-language-server/src" "$DEST_CHPL_HOME/tools/chpl-language-server/src"

  if [ ! -z "$PREFIX" ]
  then
    ln -s "$DEST_CHPL_HOME/tools/chpl-language-server/chpl-language-server" "$PREFIX/bin"/chpl-language-server
    ln -s "$DEST_CHPL_HOME/tools/chpl-language-server/chpl-shim" "$PREFIX/bin"/chpl-shim
  else
    ln -s "$DEST_CHPL_HOME/tools/chpl-language-server/chpl-language-server" "$DEST_DIR/bin/$CHPL_BIN_SUBDIR"/chpl-language-server
    ln -s "$DEST_CHPL_HOME/tools/chpl-language-server/chpl-shim" "$DEST_DIR/bin/$CHPL_BIN_SUBDIR"/chpl-shim
  fi
fi

# copy chplconfig
if [ -f chplconfig ]
then
  myinstallfileto chplconfig "$DEST_CHPL_HOME/chplconfig"
fi

# Clean up: remove any .pyc files
find "$DEST_CHPL_HOME" -name '*.pyc' -delete
# Clean up: remove any .timestamp files
find "$DEST_RUNTIME_LIB" -name '.timestamp' -delete
