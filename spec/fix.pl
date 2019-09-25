#!/usr/bin/env perl

@buffer = ( "", "", "", "", "" );

while(<>) {
  # Fix links like §\ `18.1 <#Record_Declarations>`__
  # to become :ref:`Record_Declarations`
  s/§?\\? ?`[0-9. ]*<#([^>]+)>`__/:ref:`$1`/g;

  # Fix links like `\ [Notation] <#Notation>`__
  # to become :ref:`Chapter-Notation`
  s/§?\\? ?`\[[^\]]+\] *<#([^>]+)>`__/:ref:`Chapter-$1`/g;

  # Fix chapter headings like this
  #
  # -4
  # -3 Ranges
  # -2 ======
  # -1
  #  0 [Ranges]
  # 
  # into
  #  .. _Chapter-Ranges:
  #
  #  Ranges
  #  ======
  #
  # Transform lines like [Records] into cross reference targets
  if (/^\[([^\]]+)\]/) {
    s/^\[([^\]]+)\]/.. _Chapter-$1:/;
    # Insert label and a blank line before the section header
    # Insert 1 elements into the buffer
    splice(@buffer, -3, 0, $_ . "\n");
    # Remove the last element from the buffer (should be a blank line)
    pop(@buffer);
    $_ = "";
  }
  
  $line = $_;

  # print the first element of the buffer
  # remove that element
  $first = shift @buffer;
  print $first;
  # add this line to the end of the buffer
  push @buffer, $line;
}

for (@buffer) {
  print;
}
