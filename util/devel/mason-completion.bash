#!/usr/bin/env bash

_options(){
  local optional
  local optional_abbr
  optional=$(mason "$1" --help | grep -v '^[A-Za-z]' | grep '^.*--' |      \
            sed -e 's;^\(.*--[^ ]*\).*$;\1;' | sed -e 's;-[A-Za-z],;   ;' | \
            xargs echo)
  optional_abbr=$(mason "$1" --help | grep -v '^[A-Za-z]' | grep '^.*--' |        \
          sed -e 's;^\(.*--[^ ]*\).*$;\1;' | grep ',' | sed -e 's;,.*$;;' | \
          xargs echo)
  COMPREPLY=($(compgen -W "$optional $optional_abbr" -- "${COMP_WORDS[COMP_CWORD]}" ))
}

_complete_mason_system(){
  arg=$(mason system --help | grep -v '^[A-Za-z]' | grep -v '^.*--'  | \
     grep -v '<command>' | grep -v '\[options\]'  | sed $'s/^[ \t]*//' | \
     cut -d" " -f1 | xargs echo)
  cur=${COMP_WORDS[COMP_CWORD]}
  if [[ ${COMP_CWORD} -le 2 ]]
  then
    case "$cur" in 
      -*)
        _options system 
        ;;
       *)
        COMPREPLY=($(compgen -W "$arg" "$cur"))
        ;;
    esac
  fi
  if [[ ${COMP_CWORD} -gt 2 ]]
  then
    case "$cur" in
      -*)
        _options system
        ;;
       *)
        ;;
    esac
  fi
}

_complete_mason_publish(){
  local cur
  cur=${COMP_WORDS[COMP_CWORD]}
  case "$cur" in
    -*)
      _options publish
      ;;
    *)
      COMPREPLY=($(compgen -d -S / -- "$cur"))
      ;;
  esac
}

_complete_mason_test(){
  local cur
  cur=${COMP_WORDS[COMP_CWORD]}
  case "$cur" in
    -*)
      _options test 
      ;;
    *)
      COMPREPLY=($(compgen -d -S / -- "$cur"))
      ;;
  esac
}

_complete_mason_doc(){
  COMPREPLY=($(compgen -W "--help -h" -- "$1" ))
}

_complete_mason_clean(){
  COMPREPLY=($(compgen -W "--help -h" -- "$1" ))
}

_complete_mason_env(){
  COMPREPLY=($(compgen -W "--help -h" -- "$1" ))
}

_complete_mason_search(){
  _options search
}

_complete_mason_update(){
  local cur
  cur=${COMP_WORDS[COMP_CWORD]}
  case "$cur" in
    -*) 
      _options update
      ;;
     *)
      ;;
  esac
}

_complete_mason_run(){
  local optional
  local optional_abbr
  optional=$(mason run --help | grep -v '^[A-Za-z]' | grep '^.*--' |      \
            sed -e 's;^\(.*--[^ ]*\).*$;\1;' | sed -e 's;-[A-Za-z],;   ;' | \
             sed -e 's/.*=//g' |\
             xargs echo)
  optional_abbr=$(mason run --help | grep -v '^[A-Za-z]' | grep '^.*--' |        \
          sed -e 's;^\(.*--[^ ]*\).*$;\1;' | grep ',' | sed -e 's;,.*$;;' | \
          xargs echo)
  local cur
  cur=${COMP_WORDS[COMP_CWORD]}
  case "$cur" in
    -*)
      COMPREPLY=($(compgen -W "$optional $optional_abbr" -- "${COMP_WORDS[COMP_CWORD]}" ))
      ;;
    *)
      COMPREPLY=($(compgen -d -S / -- "$cur"))
      ;;
  esac
}

_complete_mason_build(){
  local optional
  local optional_abbr
  optional=$(mason build --help | grep -v '^[A-Za-z]' | grep '^.*--' |      \
            sed -e 's;^\(.*--[^ ]*\).*$;\1;' | sed -e 's;-[A-Za-z],;   ;' | \
             sed -e 's/.*=//g' | sed -e 's/[][]//g'|  \
             xargs echo)
  optional_abbr=$(mason build --help | grep -v '^[A-Za-z]' | grep '^.*--' |        \
          sed -e 's;^\(.*--[^ ]*\).*$;\1;' | grep ',' | sed -e 's;,.*$;;' | \
          xargs echo)
  local cur
  local update_tag="--update"
  cur=${COMP_WORDS[COMP_CWORD]}
  case "$cur" in 
    -*) 
      COMPREPLY=($(compgen -W "$optional $optional_abbr $update_tag" -- "${COMP_WORDS[COMP_CWORD]}" ))
      ;;
     *)
      ;;
  esac
}

_complete_mason_add(){
  _options add
}

_complete_mason_init(){
  local cur
  cur=${COMP_WORDS[COMP_CWORD]}
  case "$cur" in
    -*)
      _options init 
      ;;
    *)
      COMPREPLY=($(compgen -d -S / -- "$cur"))
      ;;
  esac
}

_complete_mason_external(){
  arg=$(mason external --help | grep -v '^[A-Za-z]' | grep -v '^.*--'  | \
     grep -v '<command>' | grep -v '\[options\]'  | sed $'s/^[ \t]*//' | \
     cut -d" " -f1 | xargs echo)
  cur=${COMP_WORDS[COMP_CWORD]}
  if [[ ${COMP_CWORD} -le 2 ]]
  then
    case "$cur" in 
      -*)
        _options external
        ;;
       *)
        COMPREPLY=($(compgen -W "$arg" "$cur"))
        ;;
    esac
  fi
  if [[ ${COMP_CWORD} -gt 2 ]]
  then
    case "$cur" in
      -*)
        _options external
        ;;
       *)
        ;;
    esac
  fi
}

_complete_mason_new(){
  _options new
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
     grep -v '<command>' | grep -v '\[options\]'  | sed $'s/^[ \t]*//' | \
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
       grep -v '<command>' | grep -v '\[options\]'  | sed $'s/^[ \t]*//' | \
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
        ;;
      'add')
        _complete_mason_add "$cur"
        ;;
      'rm')
        _complete_mason_add "$cur"
        ;;
      'run')
        _complete_mason_run "$cur"
        ;;
      'build')
        _complete_mason_build "$cur"
        ;;
      'update')
        _complete_mason_update "$cur"
        ;;
      'search')
        _complete_mason_search "$cur"
        ;;
      'env')
        _complete_mason_env "$cur"
        ;;
      'clean')
        _complete_mason_clean "$cur"
        ;;
      'doc')
        _complete_mason_doc "$cur"
        ;;
      'test')
        _complete_mason_test "$cur"
        ;;
      'publish')
        _complete_mason_publish "$cur"
        ;;
      'external')
        _complete_mason_external "$cur"
        ;;
      'system')
        _complete_mason_system "$cur"
        ;;
    esac
  fi
}

# run _mason on hitting tab with mason
complete -F _mason mason
