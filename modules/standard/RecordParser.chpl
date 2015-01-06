/*
 * Copyright 2004-2015 Cray Inc.
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

use IO, Regexp;

/* A general purpose record reader/parser for channels.

   The assumptions on the programmer are:
   (1) The programmer has opened the file in the proper mode 

   (2) The programmer has created a record (NOT a class) that can have an arbitrary
   number of fields that can be cast from strings (i.e no subrecords, arrays etc).

   (3) The number of captures in the regex string provided, MUST match the number of
   fields in the record.

   (4) The order in which the fields in the record appear must be in the same order
   as the record being parsed. See test.chpl for an example of this.

FUTURE: 
  * Make this be able to take in a class type as opposed to a record type.
    Right now we cant take in a class type due to the future filed in 
    $CHPL_HOME/test/types/typedefs/tzakian/classConstructorsFromTypes.chpl
 */

class RecordReader {
  type t;                  // The record type to populate
  var myReader;            // Channel to read from
  var matchRegexp: regexp; // To match on the channel with
  param num_fields = __primitive("num fields", t); // Number of fields in record

  proc RecordReader(type t, myReader) {
    matchRegexp = compile(createRegexp());
  }

  proc RecordReader(type t, myReader, mRegexp) {
    matchRegexp = compile(mRegexp);
  }

  // This is a VERY loose regex, and therefore could lead to errors unless the data
  // is very nice... (but hey, the programmer wasnt willing to give us a regex..)
  proc createRegexp() {
    var accum: string = "\\s*";
    for param n in 1..num_fields {
      accum = accum + __primitive("field num to name", t, n) + "\\s*(.*?)" + "\\s*";
    }
    return accum;
  }

  iter stream_num(offst: int(64), len: int(64)) { 
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

  //  // Get one record 
  proc get() { 
    var (rec, once) = _get_internal();
    if(!once) // We havent gotten everything that we should have. 
      then halt("EOF reached -- record not populated");
    return rec;
  }

  iter stream() { 
    do {
      var (rec, once) = _get_internal();
      if (once == true) // Populated
        then yield rec;
    } while(once);
  }

  // An internal function that we use all our user visible code.
  // When called with no arguments it does no checking abut block boundaries
  // HDFS specific when offst and len are given. Checks for block boundaries for parallel file IO
  proc _get_internal(offst: int(64) = 0, len: int(64) = -1) { 
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
        var tmp = __primitive("field value by num", rec, n);
        var s: string;
        myReader.extractMatch(m(n + 1), s);
        if(s == "") 
          then __primitive("field value by num", rec, n) = tmp;
        else __primitive("field value by num", rec, n) = s:tmp.type;
        once = true;
      }
    } 
    return (rec, once);
  }

}


