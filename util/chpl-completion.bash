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
      # developer options
      local devel_opts="\
--allow-noinit-array-not-pod \
--atomics \
--auto-aggregation \
--auto-local-access \
--aux-filesys \
--baseline \
--bounds-checks \
--break-on-codegen \
--break-on-codegen-id \
--break-on-id \
--break-on-remove-id \
--break-on-resolve-id \
--cache-remote \
--cast-checks \
--cc-warnings \
--ccflags \
--checks \
--codegen \
--comm \
--comm-substrate \
--compile-time-nil-checking \
--copy-elision \
--copy-propagation \
--copyright \
--count-tokens \
--cpp-lines \
--dead-code-elimination \
--debug \
--debug-short-loc \
--default-dist \
--denormalize \
--detailed-errors \
--devel \
--div-by-zero-checks \
--dynamic \
--dynamic-auto-local-access \
--dyno \
--dyno-break-on-hash \
--dyno-debug-trace \
--dyno-gen-lib \
--dyno-scope-bundled \
--dyno-scope-production \
--dyno-scope-resolve \
--dyno-verify-serialization \
--early-deinit \
--explain-call \
--explain-call-id \
--explain-instantiation \
--explain-verbose \
--fast \
--fast-followers \
--force-vectorize \
--formal-domain-checks \
--gasnet-segment \
--gdb \
--gen-ids \
--gmp \
--gpu-arch \
--gpu-block-size \
--gpu-ptxas-enforce-optimization \
--hdr-search-path \
--help \
--help-env \
--help-settings \
--home \
--html \
--html-chpl-home \
--html-print-block-ids \
--html-user \
--html-wrap-lines \
--hwloc \
--ieee-float \
--ignore-errors \
--ignore-errors-for-pass \
--ignore-local-classes \
--ignore-nilability-errors \
--ignore-user-errors \
--incremental \
--infer-const-refs \
--infer-implements-decls \
--infer-local-fields \
--inline \
--inline-iterators \
--inline-iterators-yield-limit \
--instantiate-max \
--interleave-memory \
--interprocedural-alias-analysis \
--launcher \
--ldflags \
--lib-linkage \
--lib-search-path \
--library \
--library-cmakelists \
--library-dir \
--library-fortran \
--library-fortran-name \
--library-header \
--library-makefile \
--library-ml-debug \
--library-python \
--library-python-name \
--license \
--lifetime-checking \
--live-analysis \
--lldb \
--llvm \
--llvm-print-ir \
--llvm-print-ir-stage \
--llvm-wide-opt \
--local \
--local-checks \
--local-temp-names \
--locale-model \
--localize-global-consts \
--log \
--log-deleted-ids-to \
--log-dir \
--log-ids \
--log-module \
--log-node \
--log-pass \
--loop-invariant-code-motion \
--main-module \
--make \
--max-c-ident-len \
--mem \
--memory-frees \
--minimal-modules \
--mllvm \
--module-dir \
--munge-user-idents \
--network-atomics \
--nil-checks \
--no-allow-noinit-array-not-pod \
--no-auto-aggregation \
--no-auto-local-access \
--no-bounds-checks \
--no-cache-remote \
--no-cast-checks \
--no-cc-warnings \
--no-checks \
--no-codegen \
--no-compile-time-nil-checking \
--no-copy-elision \
--no-copy-propagation \
--no-count-tokens \
--no-cpp-lines \
--no-dead-code-elimination \
--no-debug \
--no-debug-short-loc \
--no-denormalize \
--no-detailed-errors \
--no-devel \
--no-div-by-zero-checks \
--no-dynamic-auto-local-access \
--no-dyno \
--no-dyno-debug-trace \
--no-dyno-scope-bundled \
--no-dyno-scope-production \
--no-dyno-scope-resolve \
--no-dyno-verify-serialization \
--no-early-deinit \
--no-explain-verbose \
--no-fast-followers \
--no-force-vectorize \
--no-formal-domain-checks \
--no-gen-ids \
--no-html-print-block-ids \
--no-html-wrap-lines \
--no-ieee-float \
--no-ignore-errors \
--no-ignore-errors-for-pass \
--no-ignore-local-classes \
--no-ignore-nilability-errors \
--no-ignore-user-errors \
--no-incremental \
--no-infer-const-refs \
--no-infer-implements-decls \
--no-infer-local-fields \
--no-inline \
--no-inline-iterators \
--no-interleave-memory \
--no-interprocedural-alias-analysis \
--no-library-ml-debug \
--no-lifetime-checking \
--no-live-analysis \
--no-llvm \
--no-llvm-wide-opt \
--no-local \
--no-local-checks \
--no-local-temp-names \
--no-localize-global-consts \
--no-log-ids \
--no-loop-invariant-code-motion \
--no-memory-frees \
--no-minimal-modules \
--no-munge-user-idents \
--no-nil-checks \
--no-optimize \
--no-optimize-forall-unordered-ops \
--no-optimize-loop-iterators \
--no-optimize-on-clauses \
--no-optimize-range-iteration \
--no-overload-sets-checks \
--no-override-checking \
--no-parse-only \
--no-permit-unhandled-module-errors \
--no-preserve-inlined-line-numbers \
--no-print-all-candidates \
--no-print-callgraph \
--no-print-callstack-on-error \
--no-print-code-size \
--no-print-commands \
--no-print-id-on-error \
--no-print-passes \
--no-print-search-dirs \
--no-print-unused-functions \
--no-print-unused-internal-functions \
--no-privatization \
--no-region-vectorizer \
--no-remote-serialization \
--no-remote-value-forwarding \
--no-remove-copy-calls \
--no-remove-empty-records \
--no-remove-unreachable-blocks \
--no-replace-array-accesses-with-ref-temps \
--no-report-aliases \
--no-report-auto-aggregation \
--no-report-auto-local-access \
--no-report-blocking \
--no-scalar-replacement \
--no-specialize \
--no-split-initialization \
--no-stack-checks \
--no-task-tracking \
--no-tuple-copy-opt \
--no-use-color-terminal \
--no-use-io-serializers \
--no-vectorize \
--no-verify \
--no-warn-array-of-range \
--no-warn-const-loops \
--no-warn-domain-literal \
--no-warn-int-uint \
--no-warn-special \
--no-warn-tuple-iteration \
--no-warn-unknown-attribute-toolname \
--no-warn-unstable \
--no-warn-unstable-internal \
--no-warn-unstable-standard \
--no-warnings \
--optimize \
--optimize-forall-unordered-ops \
--optimize-loop-iterators \
--optimize-on-clause-limit \
--optimize-on-clauses \
--optimize-range-iteration \
--output \
--overload-sets-checks \
--override-checking \
--parallel-make \
--parse-only \
--parser-debug \
--permit-unhandled-module-errors \
--prepend-internal-module-dir \
--prepend-standard-module-dir \
--preserve-inlined-line-numbers \
--print-additional-errors \
--print-all-candidates \
--print-callgraph \
--print-callstack-on-error \
--print-chpl-home \
--print-chpl-loc \
--print-chpl-settings \
--print-code-size \
--print-commands \
--print-dispatch \
--print-emitted-code-size \
--print-id-on-error \
--print-module-files \
--print-module-resolution \
--print-passes \
--print-passes-file \
--print-search-dirs \
--print-statistics \
--print-unused-functions \
--print-unused-internal-functions \
--privatization \
--re2 \
--region-vectorizer \
--remote-serialization \
--remote-value-forwarding \
--remove-copy-calls \
--remove-empty-records \
--remove-unreachable-blocks \
--replace-array-accesses-with-ref-temps \
--report-aliases \
--report-auto-aggregation \
--report-auto-local-access \
--report-blocking \
--report-dead-blocks \
--report-dead-modules \
--report-gpu-transform-time \
--report-inlined-iterators \
--report-inlining \
--report-optimized-forall-unordered-ops \
--report-optimized-loop-iterators \
--report-optimized-on \
--report-promotion \
--report-scalar-replace \
--report-vectorized-loops \
--savec \
--scalar-replace-limit \
--scalar-replacement \
--set \
--specialize \
--split-initialization \
--stack-checks \
--static \
--stop-after-pass \
--target-arch \
--target-compiler \
--target-cpu \
--target-platform \
--task-tracking \
--tasks \
--timers \
--tuple-copy-limit \
--tuple-copy-opt \
--use-color-terminal \
--use-io-serializers \
--using-attribute-toolname \
--vectorize \
--verify \
--version \
--warn-array-of-range \
--warn-const-loops \
--warn-domain-literal \
--warn-int-uint \
--warn-special \
--warn-tuple-iteration \
--warn-unknown-attribute-toolname \
--warn-unstable \
--warn-unstable-internal \
--warn-unstable-standard \
--warnings \
-I \
-L \
-M \
-O \
-g \
-h \
-j \
-l \
-o \
-s"

      # user options
      local nodevel_opts="\
--atomics \
--auto-aggregation \
--auto-local-access \
--aux-filesys \
--baseline \
--bounds-checks \
--cache-remote \
--cast-checks \
--ccflags \
--checks \
--codegen \
--comm \
--comm-substrate \
--copy-propagation \
--copyright \
--count-tokens \
--cpp-lines \
--dead-code-elimination \
--debug \
--devel \
--div-by-zero-checks \
--dynamic \
--dynamic-auto-local-access \
--explain-call \
--explain-instantiation \
--explain-verbose \
--fast \
--fast-followers \
--formal-domain-checks \
--gasnet-segment \
--gmp \
--hdr-search-path \
--help \
--help-env \
--help-settings \
--home \
--hwloc \
--ieee-float \
--ignore-local-classes \
--infer-local-fields \
--inline \
--inline-iterators \
--inline-iterators-yield-limit \
--instantiate-max \
--launcher \
--ldflags \
--lib-linkage \
--lib-search-path \
--license \
--live-analysis \
--llvm \
--llvm-wide-opt \
--local \
--local-checks \
--locale-model \
--loop-invariant-code-motion \
--main-module \
--make \
--max-c-ident-len \
--mem \
--mllvm \
--module-dir \
--munge-user-idents \
--network-atomics \
--nil-checks \
--no-auto-aggregation \
--no-auto-local-access \
--no-bounds-checks \
--no-cache-remote \
--no-cast-checks \
--no-checks \
--no-codegen \
--no-copy-propagation \
--no-count-tokens \
--no-cpp-lines \
--no-dead-code-elimination \
--no-debug \
--no-devel \
--no-div-by-zero-checks \
--no-dynamic-auto-local-access \
--no-explain-verbose \
--no-fast-followers \
--no-formal-domain-checks \
--no-ieee-float \
--no-ignore-local-classes \
--no-infer-local-fields \
--no-inline \
--no-inline-iterators \
--no-live-analysis \
--no-llvm \
--no-llvm-wide-opt \
--no-local \
--no-local-checks \
--no-loop-invariant-code-motion \
--no-munge-user-idents \
--no-nil-checks \
--no-optimize \
--no-optimize-forall-unordered-ops \
--no-optimize-loop-iterators \
--no-optimize-on-clauses \
--no-optimize-range-iteration \
--no-permit-unhandled-module-errors \
--no-print-all-candidates \
--no-print-callgraph \
--no-print-callstack-on-error \
--no-print-code-size \
--no-print-commands \
--no-print-passes \
--no-print-search-dirs \
--no-print-unused-functions \
--no-privatization \
--no-remote-serialization \
--no-remote-value-forwarding \
--no-remove-copy-calls \
--no-scalar-replacement \
--no-specialize \
--no-stack-checks \
--no-task-tracking \
--no-tuple-copy-opt \
--no-vectorize \
--no-warn-unknown-attribute-toolname \
--no-warn-unstable \
--no-warnings \
--optimize \
--optimize-forall-unordered-ops \
--optimize-loop-iterators \
--optimize-on-clause-limit \
--optimize-on-clauses \
--optimize-range-iteration \
--output \
--permit-unhandled-module-errors \
--print-all-candidates \
--print-callgraph \
--print-callstack-on-error \
--print-chpl-home \
--print-code-size \
--print-commands \
--print-module-files \
--print-passes \
--print-passes-file \
--print-search-dirs \
--print-unused-functions \
--privatization \
--re2 \
--remote-serialization \
--remote-value-forwarding \
--remove-copy-calls \
--savec \
--scalar-replace-limit \
--scalar-replacement \
--set \
--specialize \
--stack-checks \
--static \
--target-arch \
--target-compiler \
--target-cpu \
--target-platform \
--task-tracking \
--tasks \
--timers \
--tuple-copy-limit \
--tuple-copy-opt \
--using-attribute-toolname \
--vectorize \
--version \
--warn-unknown-attribute-toolname \
--warn-unstable \
--warnings \
-I \
-L \
-M \
-O \
-g \
-h \
-l \
-o \
-s"

      # Look for --devel or --no-devel on the command line.
      # It overrides the CHPL_DEVELOPER environment variable.
      wordnum=$[$COMP_CWORD-1]
      developt=""
      while [ $wordnum -gt 0 ] ; do
        if [ "${COMP_WORDS[$wordnum]}" == "--devel" ] ; then
          developt="true"
          break
        elif [ "${COMP_WORDS[$wordnum]}" == "--no-devel" ] ; then
          developt="false"
          break
        fi
        wordnum=$[$wordnum-1]
      done

      if [ "$developt" == "true" ] ; then
        COMPREPLY=( $( compgen -W '$devel_opts' -- $cur ))
      elif [ "$developt" == "false" ] ; then
        COMPREPLY=( $( compgen -W '$nodevel_opts' -- $cur ))
      elif [ -z $CHPL_DEVELOPER ] ; then
        COMPREPLY=( $( compgen -W '$nodevel_opts' -- $cur ))
      else
        COMPREPLY=( $( compgen -W '$devel_opts' -- $cur ))
      fi

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
        local expanded=$(expandPath $COMPREPLY)
        if [ -d $expanded ]; then
          LASTCHAR='/'
        fi
        COMPREPLY=$(printf %s%s "$COMPREPLY" "$LASTCHAR")
      else
        for ((i=0; i < ${#COMPREPLY[@]}; i++)); do
          local expanded=$(expandPath ${COMPREPLY[$i]})
          if [ -d $expanded ]; then
            COMPREPLY[$i]=${COMPREPLY[$i]}/
          fi
        done
      fi
    ;;
  esac
  return 0
}

expandPath() {
  case $1 in
    ~[+-]*)
      local content content_q
      printf -v content_q '%q' "${1:2}"
      eval "content=${1:0:2}${content_q}"
      printf '%s\n' "$content"
      ;;
    ~*)
      local content content_q
      printf -v content_q '%q' "${1:1}"
      eval "content=~${content_q}"
      printf '%s\n' "$content"
      ;;
    *)
      printf '%s\n' "$1"
      ;;
  esac
}

complete -o nospace -F _chpl chpl
