#!/usr/bin/env bash

_complete_mason_init(){
  local optional 
  local optional_abbr
  local arg
  local cur
  cur=${COMP_WORDS[COMP_CWORD]}
  arg=$(mason init --help | grep -v '^[A-Za-z]' | grep -v '^.*--'  | \
     grep -v '<command>' | grep -v '\[options\]'  | sed 's/^[ \t]*//' | \
     cut -d" " -f1 | xargs echo)
  optional=$(mason init --help | grep -v '^[A-Za-z]' | grep '^.*--' |      \
            sed -e 's;^\(.*--[^ ]*\).*$;\1;' | sed -e 's;-[A-Za-z],;   ;' | \
            xargs echo)
  optional_abbr=$(mason init --help | grep -v '^[A-Za-z]' | grep '^.*--' |        \
          sed -e 's;^\(.*--[^ ]*\).*$;\1;' | grep ',' | sed -e 's;,.*$;;' | \
          xargs echo)
  case "$cur" in
    -*)
      COMPREPLY=($(compgen -W "$optional $optional_abbr" -- "$1"))
      ;;
    *)
      COMPREPLY=($(compgen -d -S / -- "$cur"))
      ;;
  esac
}

_complete_mason_new(){
  local optional
  local optional_abbr
  optional=$(mason new --help | grep -v '^[A-Za-z]' | grep '^.*--' |      \
            sed -e 's;^\(.*--[^ ]*\).*$;\1;' | sed -e 's;-[A-Za-z],;   ;' | \
            xargs echo)
  optional_abbr=$(mason new --help | grep -v '^[A-Za-z]' | grep '^.*--' |        \
          sed -e 's;^\(.*--[^ ]*\).*$;\1;' | grep ',' | sed -e 's;,.*$;;' | \
          xargs echo)
  COMPREPLY=($(compgen -W "$optional $optional_abbr" -- "$1"))

}
_complete_mason_option(){
  local optional
  local optional_abbr
  optional=$(mason --help | grep -v '^[A-Za-z]' | grep '^.*--' |      \
            sed -e 's;^\(.*--[^ ]*\).*$;\1;' | sed -e 's;-[A-Za-z],;   ;' | \
            xargs echo)
  optional_abbr=$(mason --help | grep -v '^[A-Za-z]' | grep '^.*--' |        \
          sed -e 's;^\(.*--[^ ]*\).*$;\1;' | grep ',' | sed -e 's;,.*$;;' | \
          xargs echo)
  COMPREPLY=($(compgen -W "$optional $optional_abbr" -- "$1" ))
}

_complete_mason_command(){
  local cur
  local prev
  local arg
  cur=${COMP_WORDS[COMP_CWORD]}
  prev="${COMP_WORDS[COMP_CWORD-1]}"
  arg=$(mason --help | grep -v '^[A-Za-z]' | grep -v '^.*--'  | \
     grep -v '<command>' | grep -v '\[options\]'  | sed 's/^[ \t]*//' | \
     cut -d" " -f1 | xargs echo)
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
        _complete_mason_option "$cur"
        ;;
      *)
        _complete_mason_command "$cur"
    esac
  fi
  if [[ ${COMP_CWORD} -ge 2 ]]
  then
    local cur=${COMP_WORDS[COMP_CWORD]}
    local prev=${COMP_WORDS[COMP_CWORD-1]}
    local alt_prev=${COMP_WORDS[COMP_CWORD-2]}
    local subcmd
    local arg
    arg=$(mason --help | grep -v '^[A-Za-z]' | grep -v '^.*--'  | \
       grep -v '<command>' | grep -v '\[options\]'  | sed 's/^[ \t]*//' | \
       cut -d" " -f1 | xargs echo)

    for i in $arg
    do
      if [[ $prev == "$i" ]]
      then
        subcmd=$prev
        break;
      elif [[ $alt_prev == "$i" ]]
      then
        subcmd=$alt_prev
        break;
      fi
    done
    case "$subcmd" in 
      'new')
        _complete_mason_new "$cur"
        ;;
      'init')
        _complete_mason_init "$cur"
    esac
  fi
}

# run _mason on hitting tab with mason
complete -F _mason mason
