proc makeString() {
  var s = "hello"*2;
  return s;
}

proc f(a=makeString(), ref b=makeString(), ref c=b) {
  return (a,b,c);
}

proc g(ref a=makeString()) ref {
  return a;
}

proc h(ref a=makeString(), ref b=a) ref {
  return b;
}

proc makeTuple() {
  var s = "hello"*2;
  return (0,s);
}

proc t(offset=makeTuple()) {
  return offset;
}


proc test() {
  writeln(makeString());
  writeln(f());
  writeln(g());
  writeln(h());
  writeln(makeTuple());
  writeln(t());
}

test();
