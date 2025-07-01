import IO, JSON;

record myRec {
  var field: int;
}

var inFile = IO.open("fromJsonThrows.in", IO.ioMode.r);
var inReader = inFile.reader();

var rec: myRec;

proc main() throws {
  try {
    rec = JSON.fromJson(inReader.readAll(string), myRec);
    writeln(rec);
  }
  catch err: IllegalArgumentError {
    writeln("Caught the error correctly");
  }
}
