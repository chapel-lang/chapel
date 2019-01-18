var global = 0;

proc access() const ref {
  writeln("CONST REF");
  return global;
}

proc access() {
  writeln("VALUE");
  return global;
}

proc testConstRef() {
  const ref tmp = access(); // should use const ref version
  writeln(tmp);
}

proc testValue() {
  var tmp = access(); // should use value version
  writeln(tmp);
}


testConstRef();
testValue();
