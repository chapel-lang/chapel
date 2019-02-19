module CSV {
  record CSVIO {
    var ch: channel;
    var sep: string = ",";
    var hasHeader = true;

    class ReaderWriterMismatchError: Error {}

    proc init(filename: string, reader:bool=true, sep=",") {
      if reader then ch = openreader(filename);
                else ch = openwriter(filename);
    }
    proc init(ch: channel, sep: string=",", hasHeader=true) {
      this.ch = ch;
      this.sep = sep;
      this.hasHeader = hasHeader;
    }

    iter readCSV(type t) throws where isRecordType(t) {
      use Reflection;
      var r: t;
      var skipHeader = hasHeader;

      if ch.writing {
        throw new owned ReaderWriterMismatchError();
      }

      for l in ch.lines() {
        const line = l.strip(leading=false);
        if line.length == 0 then
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

    iter readCSV(type t...) throws {
      if ch.writing {
        throw new owned ReaderWriterMismatchError();
      }
      var r: t;
      var skipHeader = hasHeader;
      for l in ch.lines() {
        const line = l.strip(leading=false);
        if line.length == 0 then
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

    proc writeRecordCSV(r: ?t) throws where isRecordType(t) {
      use Reflection;

      if !ch.writing {
        throw new owned ReaderWriterMismatchError();
      }

      for param i in 1..numFields(t) {
        ch.write(getField(r, i));
        if i < numFields(t) then
          ch.write(sep);
      }
      ch.writeln();
    }

    proc writeRecordCSV(tup: ?t) throws where isTupleType(t) {
      if !ch.writing {
        throw new ReaderWriterMismatchError();
      }
      for param i in 1..tup.size {
        ch.write(tup(i));
        if i < tup.size then
          ch.write(sep);
      }
      ch.writeln();
    }
  }
}

module Test {
  use CSV;
  config const infile  = "",
               outfile = "";

  proc main {

    writeln("First read");
    try {
      // read and write using a tuple. The type arguments to `readCSV` define
      // the types in each row.
      var myReader = if infile == "" then stdin else openreader(infile);
      var myWriter = if outfile == "" then stdout else openwriter(outfile);
      var r = new CSVIO(myReader, hasHeader=false);
      var w = new CSVIO(myWriter);
      var myData = r.readCSV((...(4*real)), string);

      for d in myData {
        w.writeRecordCSV(d);
      }
    } catch error {
      writeln("Error: ", error);
    }

    writeln("Second read");

    try {
      // read and write using a record to declare the data types in each row
      record Row {
        var val1, val2, val3, val4: real;
        var seriesLabel: string;
      }

      var myReader = if infile == "" then stdin else openreader(infile);
      var myWriter = if outfile == "" then stdout else openwriter(outfile);
      var r = new CSVIO(myReader, hasHeader=false);
      var w = new CSVIO(myWriter);

      var myData = r.readCSV(Row);

      for d in myData {
        w.writeRecordCSV(d);
      }
    } catch error {
      writeln("Error: ", error);
    }

    try {
      // read the data into tuples like before, then copy it
      // into a 2D array and 1D array of labels
      var myReader = if infile == "" then stdin else openreader(infile);
      var myWriter = if outfile == "" then stdout else openwriter(outfile);
      var r = new CSVIO(myReader, hasHeader=false);
      var w = new CSVIO(myWriter);

      var myData = r.readCSV((...(4*real)), string);
      var A: [1..myData.numElements, 1..myData[1].size-1] real;
      var labels: [1..myData.numElements] string;

      for i in 1..myData.numElements {
        for param j in 1..myData[1].size-1 {
          A[i,j] = myData[i][j];
        }
        labels[i] = myData[i][myData[1].size];
      }

      writeln(A);
      writeln(labels);
    } catch error {
      writeln("Error: ", error);
    }
  }
}
