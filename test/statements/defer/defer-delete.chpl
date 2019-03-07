
class C {
  var x: int;
  proc deinit() {
    writeln("Destroying ", x);
  }
}

var returnEarly = false;

proc test() throws {
  var a = new unmanaged C(1);
  defer delete a;
  writeln("A");
  if returnEarly then return;
  writeln("B");
}

writeln("testing without early return");
returnEarly = false;
test();
writeln("testing with early return");
returnEarly = true;
test();

