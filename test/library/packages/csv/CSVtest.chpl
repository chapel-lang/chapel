module CSVtest {
  use IO;
  use CSV;
  config const infile  = "csvdata.txt",
               outfile = "";

  proc main {

    writeln("First read");
    try {
      // read and write using a tuple. The type arguments to `read` define
      // the types in each row.
      var myReader = if infile == "" then stdin else openreader(infile);
      var myWriter = if outfile == "" then stdout else openwriter(outfile);
      var r = new CSVIO(myReader, hasHeader=false);
      var w = new CSVIO(myWriter);
      var myData = r.read((...(4*real)), string);

      for d in myData {
        w.write(d);
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

      var myData = r.read(Row);

      for d in myData {
        w.write(d);
      }
    } catch error {
      writeln("Error: ", error);
    }

    writeln("Third read");
    try {
      // read the data into tuples like before, then copy it
      // into a 2D array and 1D array of labels
      var myReader = if infile == "" then stdin else openreader(infile);
      var myWriter = if outfile == "" then stdout else openwriter(outfile);
      var r = new CSVIO(myReader, hasHeader=false);
      var w = new CSVIO(myWriter);

      var myData = r.read((...(4*real)), string);
      var A: [1..myData.size, 1..myData[1].size-1] real;
      var labels: [1..myData.size] string;

      for i in 1..myData.size {
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

    writeln("Fourth read");
    try {
      // read and write using a tuple. The type arguments to `read` define
      // the types in each row.
      type t = ((...(4*real)), string);
      var myReader = if infile == "" then stdin else openreader(infile);
      var myWriter = if outfile == "" then stdout else openwriter(outfile);
      var r = new CSVIO(myReader, hasHeader=false);
      var w = new CSVIO(myWriter);
      var myData = r.read(t);

      for d in myData {
        w.write(d);
      }
    } catch error {
      writeln("Error: ", error);
    }

  }
}
