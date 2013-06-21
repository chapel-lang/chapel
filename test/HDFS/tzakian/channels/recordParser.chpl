use IO, Regexp;

/* A general purpose record reader/parser for channels.
   
   The assumptions on the programmer are:
   (1) The programmer has opened the file in the proper mode 

   (2) The programmer has created a record (NOT a class) that can have an arbitrary
       number of 1D fields (i.e no subrecords, arrays etc).

   (3) The number of captures in the regex string provided, MUST have the number of
       fields in the record.

   (4) The order in which the fields in the record appear must be in the same order
       as the record being parsed. See test.chpl for an example of this.

 */



class RecordReader {
  type t;                  // The record to read into 
  var myReader;
  var matchRegexp: regexp; // To match on the channel with
  param num_fields = __primitive("num fields", t); // Number of fields in record

  // FIXME Fix createRegexp()
  proc RecordReader(type t, myReader) {
      matchRegexp = compile(createRegexp());
  }

  proc RecordReader(type t, myReader, mRegexp) {
    matchRegexp = compile(mRegexp);
  }

  // As general purpose as I can think for now..
  proc createRegexp() {
    var accum: string = "\\s*";
    for param n in 1..num_fields {
      accum = accum + __primitive("field num to name", t, n) + "\\s*(.*?)" + "\\s*";
    }
    return accum;
  }

  proc _get_internal() { 
    var rec: t; // create record
    var once = false;
    for m in myReader.matches(matchRegexp, num_fields, 1) {
      for param n in 1..num_fields {
        var tmp = __primitive("field value by num", rec, n);
        var s: string;
        myReader.extractMatch(m(n + 1), s);
        __primitive("field value by num", rec, n) = s:tmp.type;
        once = true;
      }
    }
    return (rec, once);
  }

  // signal error if after EOF?
  proc get() { 
    var (rec, once) = _get_internal();
    return rec;
  }

  iter stream() { // Make a leader follower for this
    while (true) {
      var (rec, once) = _get_internal();
      if (once == true)
        then yield rec;
      else break;
    }
  }
}


