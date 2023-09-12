# TODO: I would like to wrap all this code into it's own function and 'local'
# the variables it uses so as to not risk polluting the user's environment.
# Unfortunatly, doing so runs into this issue
# https://stackoverflow.com/questions/76726843/different-behavior-in-bash-with-export-in-a-script-depending-on-if-it-is-sourc
# because in /cray/css/users/chapelu/setup_cmake_nightly.bash we end up doing
# something like this: MODULEPATH='' source
# $SPACK_ROOT/share/spack/setup-env.sh, which ends up inadvertedly exporting
# the MODULEPATH variable if called from within a function.

# ALSO BE AWARE: This script has a .bash extension but ideally I'd like this to
# work in zsh as well.

# From
# <https://stackoverflow.com/questions/2683279/how-to-detect-if-a-script-is-being-sourced>.
# This script only works if its been sourced so warn the user if it's been
# invoked some other way.
(
  [[ -n $ZSH_VERSION && $ZSH_EVAL_CONTEXT =~ :file$ ]] ||
  [[ -n $KSH_VERSION && "$(cd -- "$(dirname -- "$0")" && pwd -P)/$(basename -- "$0")" != "$(cd -- "$(dirname -- "${.sh.file}")" && pwd -P)/$(basename -- "${.sh.file}")" ]] ||
  [[ -n $BASH_VERSION ]] && (return 0 2>/dev/null)
) && _chplSetupEnv_sourced=1 || _chplSetupEnv_sourced=0

if [[ "$_chplSetupEnv_sourced" == "0" ]]; then
  >&2 echo "In order to work correctly, the chplSetupEnv must be sourced (invoked like '. chplSetupEnv' or 'source chplSetupEnv')."
  exit 1
fi

_chplSetupEnv_suffix="bash"
_chplSetupEnv_pathsToRun=("$CHPL_HOME/util/test/chplSetup" "/data/cf/chapel/chplSetup/" "/chapel/data/chaptools/chplSetup" "/cray/css/users/chapelu/chplSetup" "$HOME/.chplSetup" "$(pwd)/chplSetup")
_chplSetupEnv_shortHost=$(hostname -s)
_chplSetupEnv_featureSet=""
_chplSetupEnv_dryRun=""
_chplSetupEnv_noBaseCfg=""

# TODO: make this less hacky, use getopts
for _chplSetupEnv_VAR in "$@"; do
  if [[ "$_chplSetupEnv_VAR" == --* ]]; then
    if [[ "$_chplSetupEnv_VAR" == --dry-run ]]; then
      _chplSetupEnv_dryRun=y
    elif [[ "$_chplSetupEnv_VAR" == --no-base-cfg ]]; then
      _chplSetupEnv_noBaseCfg=y
    else
      >&2 echo "Unknown argument $var"
      return 1
    fi
  else
    _chplSetupEnv_featureSet="$_chplSetupEnv_VAR"
  fi
done

# Fail to do this and the arguments will get propoagated to any scripts we
# source:
shift $#

if [[ ! "$_chplSetupEnv_noBaseCfg" == "y" ]]; then
  _chplSetupEnv_featureSet="base:$_chplSetupEnv_featureSet"
fi

if [[ ! "$_chplSetupEnv_dryRun" == "y" ]]; then
  OLD_CHPL_HOME="$CHPL_HOME"
fi

_chplSetupEnv_foundFeature=""
for _chplSetupEnv_feature in $(echo $_chplSetupEnv_featureSet | sed "s/:/ /g"); do
  _chplSetupEnv_foundFeature=""
  for _chplSetupEnv_p in ${_chplSetupEnv_pathsToRun[@]}; do
    if [ ! -d "$_chplSetupEnv_p" ]; then
      continue
    fi

    # Run <feature>.bash
    if [ -f "$_chplSetupEnv_p/$_chplSetupEnv_feature.$_chplSetupEnv_suffix" ]; then
      _chplSetupEnv_foundFeature="y"
      if [[ "$_chplSetupEnv_dryRun" == "y" ]]; then
        echo -e "\n# --- RUN $_chplSetupEnv_p/$_chplSetupEnv_feature.$_chplSetupEnv_suffix ---"
        cat  "$_chplSetupEnv_p/$_chplSetupEnv_feature.$_chplSetupEnv_suffix"
      else
        source "$_chplSetupEnv_p/$_chplSetupEnv_feature.$_chplSetupEnv_suffix"
      fi
    fi

    # Run <feature>.<hostname>.bash
    if [ -f "$_chplSetupEnv_p/$_chplSetupEnv_feature.$_chplSetupEnv_shortHost.$_chplSetupEnv_suffix" ]; then
      _chplSetupEnv_foundFeature="y"
      if [[ "$_chplSetupEnv_dryRun" == "y" ]]; then
        echo -e "\n# --- RUN $_chplSetupEnv_p/$_chplSetupEnv_feature.$_chplSetupEnv_shortHost.$_chplSetupEnv_suffix ---"
        cat  "$_chplSetupEnv_p/$_chplSetupEnv_feature.$_chplSetupEnv_shortHost.$_chplSetupEnv_suffix"
      else
        source  "$_chplSetupEnv_p/$_chplSetupEnv_feature.$_chplSetupEnv_shortHost.$_chplSetupEnv_suffix"
      fi
    fi
  done

  if [[ -z "$_chplSetupEnv_foundFeature" ]]; then
    >&2 echo "chplSetup was unable to find any files for feature: $_chplSetupEnv_feature"
    exit 1
  fi
done

if [[ ! "$_chplSetupEnv_dryRun" == "y" ]]; then
  source "$OLD_CHPL_HOME/util/setchplenv.bash"
fi

# clean up variables introduced in this script
unset _chplSetupEnv_suffix
unset _chplSetupEnv_pathsToRun
unset _chplSetupEnv_shortHost
unset _chplSetupEnv_featureSet
unset _chplSetupEnv_dryRun
unset _chplSetupEnv_noBaseCfg
unset _chplSetupEnv_VAR
unset _chplSetupEnv_feature
unset _chplSetupEnv_p
