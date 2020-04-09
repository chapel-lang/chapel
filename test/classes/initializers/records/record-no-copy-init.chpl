// This test verifies that the compiler does not attempt
// to rely on a copy initializer for MyRec.
//
// There would be a compile time failure if the compiler attempted to
// introduce a copy operation as MyRec does not define a copy initializer.


record MyRec {
  var x : int;

  proc init() {
    writeln("default init");
  }
}


proc main() {
  var x = new MyRec();         // This is pure initialization

  writeln(x);

  var y = makeRecord();        // This is a "move" rather than a "copy"

  writeln(y);
}


// A local record is "initialized" and then "moved" to the caller
proc makeRecord() {
  return new MyRec();
}



