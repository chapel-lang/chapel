#/usr/bin/env bash

_complete_mason_new(){
  local optional=`mason new --help | grep -v '^[A-Za-z]' | grep '^.*--' |      \
            sed -e 's;^\(.*--[^ ]*\).*$;\1;' | sed -e 's;-[A-Za-z],;   ;' | \
                      xargs echo`
  local optional_abbr=`mason new --help | grep -v '^[A-Za-z]' | grep '^.*--' |        \
          sed -e 's;^\(.*--[^ ]*\).*$;\1;' | grep ',' | sed -e 's;,.*$;;' | \
                  xargs echo`
  COMPREPLY=($(compgen -W "$optional $optional_abbr" -- "$1"))

}
_complete_mason_option(){
  local optional=`mason --help | grep -v '^[A-Za-z]' | grep '^.*--' |      \
            sed -e 's;^\(.*--[^ ]*\).*$;\1;' | sed -e 's;-[A-Za-z],;   ;' | \
                      xargs echo`
  local optional_abbr=`mason --help | grep -v '^[A-Za-z]' | grep '^.*--' |        \
          sed -e 's;^\(.*--[^ ]*\).*$;\1;' | grep ',' | sed -e 's;,.*$;;' | \
                  xargs echo`
  COMPREPLY=($(compgen -W "$optional $optional_abbr" -- $1 ))
}

_complete_mason_command(){
  local cur=${COMP_WORDS[COMP_CWORD]}
  local prev="${COMP_WORDS[COMP_CWORD-1]}"
  local arg=`mason --help | grep -v '^[A-Za-z]' | grep -v '^.*--'  | \
     grep -v '<command>' | grep -v '\[options\]'  | sed 's/^[ \t]*//' | \
     cut -d" " -f1 | xargs echo`
    COMPREPLY=($(compgen -W "$arg" "${COMP_WORDS[1]}"))
}

_mason(){
  local cur
  COMPREPLY=()
  cur=${COMP_WORDS[COMP_CWORD]}
  if [[ ${COMP_CWORD} -lt 2 ]]
  then
    case "$cur" in
      -*)
        _complete_mason_option $cur 
        ;;
      *)
        _complete_mason_command $cur
    esac
  fi
  if [[ ${COMP_CWORD} -ge 2 ]]
  then
    local cur=${COMP_WORDS[COMP_CWORD]}
    local prev=${COMP_WORDS[COMP_CWORD-1]}
    echo ""
    echo $cur "curr" 
    echo $prev "prev"
    case $prev in 
      'new')
        _complete_mason_new $cur
        ;;
    esac
  fi
}

# run _mason on hitting tab with mason
complete -F _mason mason
