
function echo_and_run() {
  echo "$@"
  env "$@"
}
export -f echo_and_run
