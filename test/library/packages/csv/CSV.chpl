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

/* Support for reading and writing Comma Separated Value files.

   This module provides the :record:`CSVIO` type for reading and writing
   CSV files.
 */
module CSV {
  use IO;
  /* The `CSVIO` record can be initialized with a reader or writer channel
     for reading or writing CSV files.
   */
  record CSVIO {
    var ch: channel;
    var sep: string;
    var hasHeader: bool;

    /* Initialize a CSVIO record.
       :arg ch: The channel to read from or write to.
       :arg sep: (optional) The delimiter to separate fields
       :arg hasHeader: (optional) If true, treat the first line of data
                       as a header and skip it
     */
    proc init(ch: channel, sep: string=",", hasHeader: bool=false) {
      this.ch = ch;
      this.sep = sep;
      this.hasHeader = hasHeader;
    }
    /* Read a CSV file with lines matching the types of the fields in a record
     */
    iter read(type t) throws where isRecord(t) && t != string {
      use Reflection;
      var r: t;
      var skipHeader = hasHeader;
      if ch.writing then compilerError("reading from a writing channel");

      for l in ch.lines() {
        const line = l.strip(leading=false);
        if line.size == 0 then
          continue;
        const vals = line.split(sep);
        for param i in 1..numFields(t) {
          getFieldRef(r, i) = vals[i]: getField(r, i).type;
        }
        if skipHeader {
          skipHeader = false;
        } else {
          yield r;
        }
      }
    }

    pragma "no doc"
    proc isSpecialCaseType(type t) param {
      return t == string || isRecord(t) || isTuple(t);
    }

    /* Read a CSV file with fields of the types given by
       the arguments to the function
     */
    iter read(type t...) throws where t.size > 1 || !isSpecialCaseType(t(1)) {
      if ch.writing then compilerError("reading from a writing channel");

      var r: t;
      var skipHeader = hasHeader;
      for l in ch.lines() {
        const line = l.strip(leading=false);
        if line.size == 0 then
          continue;
        const vals = line.split(sep);
        for param i in 1..t.size {
          r(i) = vals[i]: t(i);
        }
        if skipHeader {
          skipHeader = false;
        } else {
          yield r;
        }
      }
    }

    /* Read a CSV file with fields of the types given
       in the tuple argument to the function
     */
    iter read(type t) throws where isTuple(t) {
      for r in read((...t)) {
        yield r;
      }
    }

    /* Read a CSV file with arbitrarily many rows and columns. Returns the
       data as strings in a 2D array. */
    proc read(type t: string) throws {
      if ch.writing then compilerError("reading from a writing channel");
      var r: t;
      var skipHeader = hasHeader;

      var lines = ch.lines();
      var firstLine = lines[1];
      var vals = firstLine.strip().split(sep);
      const numRows = if skipHeader then lines.size - 1
                                    else lines.size;
      var A: [1..numRows, 1..vals.size] string;

      if !skipHeader {
        A[1, ..] = vals;
      }

      for i in 2..lines.size {
        const line = lines[i].strip(leading=false);
        if line.size == 0 then
          continue;
        const vals = line.split(sep);
        const row = if skipHeader then i-1 else i;
        A[row, ..] = vals;
      }
      return A;
    }


    /* Write a record to the channel owned by this `CSVIO` instance
       resulting in a single row being added to the channel.
     */
    proc write(r: ?t) throws where isRecord(t) {
      use Reflection;
      if !ch.writing then compilerError("writing to a reading channel");

      for param i in 1..numFields(t) {
        ch.write(getField(r, i));
        if i < numFields(t) then
          ch.write(sep);
      }
      ch.writeln();
    }

    /* Write a tuple to the channel owned by this `CSVIO` instance
       resulting in a single row being added to the channel.
     */
    proc write(tup: ?t) throws where isTuple(t) {
      if !ch.writing then compilerError("writing to a reading channel");

      for param i in 1..tup.size {
        ch.write(tup(i));
        if i < tup.size then
          ch.write(sep);
      }
      ch.writeln();
    }
  }
}
