#!/usr/bin/env perl

@buffer = ( "", "", "", "", "" );

print
".. _chapel-spec:

Chapel Language Specification
|||||||||||||||||||||||||||||


";

while(<>) {
  $line = $_;
  # Fix code blocks of a type annotated
  # -2  BLOCK-chapel
  # -1
  #  0  ::
  #
  #     some-code
  if (/^\s*::$/ and $buffer[-2] =~ /(BLOCK-[a-z0-9-]+)$/) {
    $type = $1;
    $type =~ s/^\s+|\s+$//g;
    $spacing = $line;
    chomp $spacing;
    $spacing =~ s/::$//g;
    $buffer[-2] =~ s/(BLOCK-[a-z0-9-]+)$//g;

    if ($type eq "BLOCK-chapel") {
      $_ = $spacing . ".. code-block:: chapel\n";
    } elsif ($type eq "BLOCK-protohead") {
      $_ = $spacing . ".. code-block:: chapel\n";
    } elsif ($type eq "BLOCK-numbered-chapel") {
      $_ = $spacing . ".. code-block:: chapel\n   :linenos:\n";
    } elsif ($type eq "BLOCK-syntax") {
      $_ = $spacing . ".. code-block:: syntax\n";
    } elsif ($type eq "BLOCK-syntaxdonotcollect") {
      $_ = $spacing . ".. code-block:: syntaxdonotcollect\n";
    } elsif ($type eq "BLOCK-chapelprintoutput") {
      $_ = $spacing . "::\n";
    } elsif ($type eq "BLOCK-commandline") {
      $_ = $spacing . ".. code-block:: bash\n";
    } elsif ($type eq "BLOCK-invisible" or $type =~ /BLOCK-test-/) {
      $_ = $spacing . ".. $type\n";
    } else {
      die "Unknown BLOCK '$type'";
    }
  }

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
