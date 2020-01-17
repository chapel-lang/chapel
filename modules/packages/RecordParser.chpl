/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/*

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

 */
module RecordParser {

use IO, Regexp, Reflection;


/* A class providing the ability to read records matching a regular expression.
 */
//   Future work is to make this be able to take in a class type as opposed to a
//   record type.  Right now we cant take in a class type due to the future filed
//   in test/types/typedefs/tzakian/classConstructorsFromTypes.chpl
class RecordReader {
  /* The record type to populate */
  type t;
  /* The channel to read from */
  var myReader;
  /* The regular expression to read (using match on the channel) */
  var matchRegexp: regexp(string);
  pragma "no doc"
  param num_fields = numFields(t); // Number of fields in record

  /* Create a RecordReader to match an auto-generated regular expression
     for a record created by the :proc:`createRegexp` routine.

     :arg t: the record type to read
     :arg myReader: the channel to read from
   */
  proc init(type t, myReader) /*throws*/ {
    this.t = t;
    this.myReader = myReader;
    // TODO: remove the following once we can throw from init() calls
    this.complete();
    try! {
      this.matchRegexp = compile(createRegexp());
    }
  }

  /* Create a RecordReader to read using a passed regular expression.

     :arg t: the record type to read
     :arg myReader: the channel to read from
     :arg mRegexp: the regular expression to read. This argument
                   currently must be a string, but in the future might be a
                   compiled regular expression.
   */
  proc init(type t, myReader, mRegexp) /* throws */ {
    this.t = t;
    this.myReader = myReader;
    // TODO: remove the following once we can throw from init() calls
    this.complete();
    try! {
        this.matchRegexp = compile(mRegexp);
    }
  }

  /* Create a string regular expression for the record type :type:`t` attached to
     this RecordReader.

     The created regular expression will search for
     ``<fieldName1> <spaces> <fieldValue1> <spaces>``
  */
  proc createRegexp() {
    // This is a VERY loose regex, and therefore could lead to errors unless the
    // data is very nice... (but hey, the programmer wasn't willing to give us a
    // regex..)
    var accum: string = "\\s*";
    for param n in 1..num_fields {
      accum = accum + getFieldName(t, n) + "\\s*(.*?)" + "\\s*";
    }
    return accum;
  }

  /* Yield records for the range offst..offst+len, but assumes that the
     channel is already at offst.

     :arg offst: the current position of the channel
     :arg len: the number of bytes to read
   */
  iter stream_num(offst: int(64), len: int(64)) {
    try! { // TODO -- should be throws instead, once that is working
      do {
        var (rec, once) = _get_internal(offst, len);
        if (once == true) {
          if (myReader.offset() >= offst+len) { // rec.end >= start + len
            // So yield and break
            yield rec;
            break;
          }
          yield rec;
        }
      } while(once);
    }
  }

  /* Read the next record */
  proc get() throws {
    var (rec, once) = _get_internal();
    if(!once) // We havent gotten everything that we should have.
      then halt("EOF reached -- record not populated");
    return rec;
  }

  /* Yield the records read */
  iter stream() {
    try! { // TODO -- should be throws, once that is working for iterators
      do {
        var (rec, once) = _get_internal();
        if (once == true) // Populated
          then yield rec;
      } while(once);
    }
  }

  /*

     An internal function that we use with all our user visible code.  When
     called with no arguments it does no checking about block boundaries.

     HDFS specific when offst and len are given. Checks for block boundaries
     for parallel file IO

   */
  pragma "no doc"
  proc _get_internal(offst: int(64) = 0, len: int(64) = -1) throws {
    var rec: t; // create record
    var once = false; // We havent populated yet
    // This will only loop through  at most one time before returning
    // FEATURE REQUEST: Make this so we don't need a for loop here
    for m in myReader.matches(matchRegexp, num_fields, 1) {
      if (((m(1).offset) >= offst+len) && len != -1) { // rec.start >= start + len
        // Then break and dont return any record
        return (rec, false);
      }
      for param n in 1..num_fields {
        var tmp = getField(rec, n);
        var s: string;
        ref dst = getFieldRef(rec, n);
        myReader.extractMatch(m(n + 1), s);
        if s == "" then
          dst = tmp;
        else if tmp.type == string then
          dst = s;
        else
          dst = s:tmp.type;
        once = true;
      }
    }
    return (rec, once);
  }

}

}
