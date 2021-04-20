use Set;

class C { var x = 0; }

record r {
  var x = new shared C();
  proc init() { writeln('default init'); }
  proc init=(other: r) { writeln('init='); }
  proc deinit() { writeln('deinit'); }
}

operator r.=(ref lhs: r, rhs: r) { writeln('assign'); }

proc create(type T): T where isPrimitiveType(T) {
  return 0:T;
}

proc create(type T) {
  return new T();
}

proc testAddToSetRemote(type T) {
  writeln('Adding to set of ' + T:string + ' remotely');
  var s: set(T);
  on Locales[1] {
    var x = create(T);
    s.add(x);
  }
  writeln(s);
}

proc testAddToSetLocal(type T) {
  writeln('Adding to set of ' + T:string + ' locally');
  var s: set(T);
  var x = create(T);
  s.add(x);
  writeln(s);
}

proc testAddToSetLocalAndRemote(type T) {
  testAddToSetRemote(T);
  testAddToSetLocal(T);
}

proc test() {
  testAddToSetLocalAndRemote(int); 
  testAddToSetLocalAndRemote(real);
  testAddToSetLocalAndRemote(string);
  testAddToSetLocalAndRemote(bytes);
  testAddToSetLocalAndRemote(r);
  testAddToSetLocalAndRemote(shared C);
  testAddToSetLocalAndRemote(owned C);
}
test();

