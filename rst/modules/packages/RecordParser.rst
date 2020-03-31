.. default-domain:: chpl

.. module:: RecordParser
   :synopsis: Read records using regular expressions.

RecordParser
============
**Usage**

.. code-block:: chapel

   use RecordParser;



Read records using regular expressions.

A general purpose record reader/parser for channels. Uses a regular expression
to capture portions of the input, and then assigns each capture to each
record field.

This module makes the following assumptions:

1. The file has been opened for reading and a reader channel has been created

2. The programmer has created a record (NOT a class) in which all fields
   can be cast from strings (i.e no subrecords, arrays etc).

3. The number of captures in the regex string provided MUST match the number
   of fields in the record.

4. The order in which the fields in the record appear must be in the same order
   as the record being parsed.

Example 1
---------

.. code-block:: chapel

  use RecordParser;

  record Bar {
    var beer: string;
    var Name: string;
  }

  var f = open("input1.txt", iomode.rw);
  var fr = f.reader();

  var M = new RecordReader(Bar, fr);

  var c = M.get();
  writeln("The first record read is ", c);

  writeln("The rest of the records are:");
  for r in M.stream() do
    writeln(r);


Example 2
---------

.. code-block:: chapel

  use RecordParser;

  record Beer {
    var name:  string;
    var beerId: int;
    var brewerId: int;
    var ABV: real;
    var style: string;
    var appearance: real;
    var aroma: real;
    var palate: real;
    var taste: real;
    var overall: real;
    var time:  int;
    var profileName:  string;
    var text: string;
  }


  var strt = "\\s*beer/name: (.*)\\s*beer/beerId: (.*)\\s*beer/brewerId: (.*)\\s*beer/ABV: (.*)\\s*beer/style: (.*)\\s*review/appearance: (.*)\\s*review/aroma: (.*)\\s*review/palate: (.*)\\s*review/taste: (.*)\\s*review/overall: (.*)\\s*review/time: (.*)\\s*review/profileName: (.*)\\s*review/text: (.*)";

  var N = new RecordReader(Beer, ffr, strt);
  writeln("========== test of stream() ==============");
  for r in N.stream() do
    writeln(r);

RecordParser Types and Functions
--------------------------------

 

.. class:: RecordReader

   .. attribute:: type t

      The record type to populate 

   .. attribute:: var myReader

      The channel to read from 

   .. attribute:: var matchRegexp: regexp(string)

      The regular expression to read (using match on the channel) 

   .. method:: proc init(type t, myReader)

      Create a RecordReader to match an auto-generated regular expression
      for a record created by the :proc:`createRegexp` routine.
      
      :arg t: the record type to read
      :arg myReader: the channel to read from
      

   .. method:: proc init(type t, myReader, mRegexp)

      Create a RecordReader to read using a passed regular expression.
      
      :arg t: the record type to read
      :arg myReader: the channel to read from
      :arg mRegexp: the regular expression to read. This argument
                    currently must be a string, but in the future might be a
                    compiled regular expression.
      

   .. method:: proc createRegexp()

      Create a string regular expression for the record type :type:`t` attached to
      this RecordReader.
      
      The created regular expression will search for
      ``<fieldName1> <spaces> <fieldValue1> <spaces>``
      

   .. itermethod:: iter stream_num(offst: int(64), len: int(64))

      Yield records for the range offst..offst+len, but assumes that the
      channel is already at offst.
      
      :arg offst: the current position of the channel
      :arg len: the number of bytes to read
      

   .. method:: proc get() throws

      Read the next record 

   .. itermethod:: iter stream()

      Yield the records read 

