var globalInt = 0;
const  otherGlobal = 22;

proc someFunction() {
  return 22;
}

proc returnRef2() ref {
  return globalInt;
}

proc returnRef2() const ref {
  return otherGlobal;
}


returnRef2() = 1;      // resolves to 'ref' return intent version
var y = returnRef2(); // resolves to default/const ref return intent

writeln(globalInt);
writeln(y);

