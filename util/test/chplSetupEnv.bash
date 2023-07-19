chplSetupWrapperFunc () {
  local suffix="bash"
  local pathsToRun=("/data/cf/chapel/chplSetup/" "/chapel/data/chaptools/chplSetup" "/cray/css/users/chapelu/chplSetup" "$HOME/.chplSetup" "$(pwd)/chplSetup")
  local shortHost=$(hostname -s)
  local featureSet=""
  local dryRun=""

  # TODO: make this less hacky, use getopts
  for VAR in "$@"; do
    if [[ "$VAR" == --* ]]; then
      if [[ "$VAR" == --dry-run ]]; then
        dryRun=y
      else
        >&2 echo "Unknown argument $var"
        return 1
      fi
    else
      featureSet="$VAR"
    fi
  done
  featureSet="base:$featureSet"

  echo "FEATURE SETUP: $featureSet"
  echo "DRY_RUN: $dryRun"

  for feature in ${featureSet//:/ }; do
    for p in ${pathsToRun[@]}; do
      if [ ! -d "$p" ]; then
        continue
      fi

      # Run <feature>.bash
      if [ -f "$p/$feature.$suffix" ]; then
        if [[ "$dryRun" == "y" ]]; then
          echo -e "\n# --- RUN $p/$feature.$suffix ---"
          cat  "$p/$feature.$suffix"
        else
          source "$p/$feature.$suffix"
        fi
      fi
        
      # Run <feature>.<hostname>.bash
      if [ -f "$p/$feature.$shortHost.$suffix" ]; then
        if [[ "$dryRun" == "y" ]]; then
          echo -e "\n# --- RUN $p/$feature.$shortHost.$suffix ---"
          cat  "$p/$feature.$shortHost.$suffix"
        else
          source  "$p/$feature.$shortHost.$suffix"
        fi
      fi
    done
  done
}

chplSetupWrapperFunc $@
