# file: chpl-completion
# chpl parameter completion

# Tab autocompletion for chpl options and directories/.chpl files in bash
# Source this file to enable this autocompletion.  To automatically enable
# autocompletion, source this file from your .bashrc file.

# This file was generated with the script:
# $CHPL_HOME/util/devel/gen-chpl-bash-completion

# This is modeled after the example at:
# http://tldp.org/LDP/abs/html/tabexpansion.html
# and Dennis's answer to this question:
# https://superuser.com/questions/564716/bash-completion-for-filename-patterns-or-directories

_chpl ()
{
  local cur
  COMPREPLY=()
  cur=${COMP_WORDS[COMP_CWORD]}
  case "$cur" in
    -*)
      # The following lists were generated with the commands preceding them.

      # single letter options
      local single="-M -g -I -l -L -O -o -s -h -t -d -D"

      # positive options + negative-only options
      local positive="--count-tokens --main-module --module-dir --print-code-size --print-module-files --print-search-dirs --local --baseline --cache-remote --copy-propagation --dead-code-elimination --fast --fast-followers --ieee-float --ignore-local-classes --inline --inline-iterators --inline-iterators-yield-limit --live-analysis --loop-invariant-code-motion --optimize-loop-iterators --optimize-on-clauses --optimize-on-clause-limit --privatization --remote-value-forwarding --remote-serialization --remove-copy-calls --scalar-replacement --scalar-replace-limit --tuple-copy-opt --tuple-copy-limit --use-noinit --infer-local-fields --vectorize --no-checks --bounds-checks --cast-checks --div-by-zero-checks --formal-domain-checks --local-checks --nil-checks --stack-checks --codegen --cpp-lines --max-c-ident-len --munge-user-idents --savec --ccflags --debug --dynamic --hdr-search-path --ldflags --lib-linkage --lib-search-path --optimize --specialize --output --static --llvm --llvm-wide-opt --mllvm --print-commands --print-passes --print-passes-file --devel --explain-call --explain-instantiation --explain-verbose --instantiate-max --print-callgraph --print-callstack-on-error --print-unused-functions --set --permit-unhandled-module-errors --task-tracking --warn-const-loops --warn-special --warn-domain-literal --warn-tuple-iteration --warnings --home --atomics --network-atomics --aux-filesys --comm --comm-substrate --gasnet-segment --gmp --hwloc --launcher --locale-model --make --mem --regexp --target-arch --target-compiler --target-platform --tasks --timers --copyright --help --help-env --help-settings --license --version --cc-warnings --gen-ids --html --html-user --html-wrap-lines --html-print-block-ids --html-chpl-home --log --log-dir --log-ids --log-module --log-pass --log-node --llvm-print-ir --llvm-print-ir-stage --verify --parse-only --parser-debug --debug-short-loc --print-emitted-code-size --print-module-resolution --print-dispatch --print-statistics --report-inlining --report-dead-blocks --report-dead-modules --report-optimized-loop-iterators --report-inlined-iterators --report-order-independent-loops --report-optimized-on --report-promotion --report-scalar-replace --break-on-id --break-on-delete-id --break-on-codegen --break-on-codegen-id --default-dist --explain-call-id --break-on-resolve-id --denormalize --gdb --lldb --force-initializers --heterogeneous --ignore-errors --ignore-errors-for-pass --library --localize-global-consts --local-temp-names --log-deleted-ids-to --memory-frees --preserve-inlined-line-numbers --print-id-on-error --print-unused-internal-functions --remove-empty-records --remove-unreachable-blocks --replace-array-accesses-with-ref-temps --incremental --minimal-modules --print-chpl-settings --user-constructor-error --print-chpl-home"

      # --no- versions of optionally negative options
      local negative="--no-count-tokens --no-print-code-size --no-print-search-dirs --no-local --no-cache-remote --no-copy-propagation --no-dead-code-elimination --no-fast-followers --no-ieee-float --no-ignore-local-classes --no-inline --no-inline-iterators --no-live-analysis --no-loop-invariant-code-motion --no-optimize-loop-iterators --no-optimize-on-clauses --no-privatization --no-remote-value-forwarding --no-remote-serialization --no-remove-copy-calls --no-scalar-replacement --no-tuple-copy-opt --no-use-noinit --no-infer-local-fields --no-vectorize --no-bounds-checks --no-cast-checks --no-div-by-zero-checks --no-formal-domain-checks --no-local-checks --no-nil-checks --no-stack-checks --no-codegen --no-cpp-lines --no-munge-user-idents --no-debug --no-optimize --no-specialize --no-llvm --no-llvm-wide-opt --no-print-commands --no-print-passes --no-devel --no-explain-verbose --no-print-callgraph --no-print-callstack-on-error --no-print-unused-functions --no-permit-unhandled-module-errors --no-task-tracking --no-warn-const-loops --no-warn-special --no-warn-domain-literal --no-warn-tuple-iteration --no-warnings --no-cc-warnings --no-gen-ids --no-html-wrap-lines --no-html-print-block-ids --no-log-ids --no-verify --no-parse-only --no-debug-short-loc --no-denormalize --no-force-initializers --no-ignore-errors --no-ignore-errors-for-pass --no-localize-global-consts --no-local-temp-names --no-memory-frees --no-preserve-inlined-line-numbers --no-print-id-on-error --no-print-unused-internal-functions --no-remove-empty-records --no-remove-unreachable-blocks --no-replace-array-accesses-with-ref-temps --no-incremental --no-minimal-modules --no-user-constructor-error"

      COMPREPLY=( $( compgen -W '$single $positive $negative' -- $cur ))

      if [ ${#COMPREPLY[@]} = 1 ]; then
        # Add a space after a fully matching argument
        COMPREPLY=$(printf %q%s "$COMPREPLY" ' ')
      fi
    ;;
    *)
      # Add a slash after a directory match and a space after a filename match
      local IFS=$'\n'
      local LASTCHAR=' '
      COMPREPLY=( $( compgen -o plusdirs -f -X '!*.chpl' -- "${COMP_WORDS[COMP_CWORD]}" ) )

      if [ ${#COMPREPLY[@]} = 1 ]; then
        if [ -d "$COMPREPLY" ]; then
          LASTCHAR='/'
        fi
        COMPREPLY=$(printf %q%s "$COMPREPLY" "$LASTCHAR")
      else
        for ((i=0; i < ${#COMPREPLY[@]}; i++)); do
          if [ -d "${COMPREPLY[$i]}" ]; then
            COMPREPLY[$i]=${COMPREPLY[$i]}/
          fi
        done
      fi
    ;;
  esac
  return 0
}

complete -o nospace -F _chpl chpl
