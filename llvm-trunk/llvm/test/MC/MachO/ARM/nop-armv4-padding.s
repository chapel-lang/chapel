@ RUN: llvm-mc -triple armv4-apple-darwin %s -filetype=obj -o %t.obj
@ RUN: macho-dump --dump-section-data < %t.obj > %t.dump
@ RUN: FileCheck %s < %t.dump

x:
      add r0, r1, r2
      .align 4
      add r0, r1, r2

@ CHECK: ('_section_data', '020081e0 0000a0e1 0000a0e1 0000a0e1 020081e0')
