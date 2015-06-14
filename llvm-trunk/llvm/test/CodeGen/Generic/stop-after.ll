; RUN: llc < %s -debug-pass=Structure -stop-after=loop-reduce -o /dev/null 2>&1 | FileCheck %s -check-prefix=STOP
; RUN: llc < %s -debug-pass=Structure -start-after=loop-reduce -o /dev/null 2>&1 | FileCheck %s -check-prefix=START

; STOP: -loop-reduce
; STOP: Loop Strength Reduction
; STOP-NEXT: Machine Function Analysis
; STOP-NEXT: MIR Printing Pass

; START: -machine-branch-prob -gc-lowering
; START: FunctionPass Manager
; START-NEXT: Lower Garbage Collection Instructions
