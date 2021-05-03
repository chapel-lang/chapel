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

proc testAddLocalElementRemote(type T) {
  writeln('Adding local element ' + T:string + ' remotely');
  var s: set(T);
  var x = create(T);
  on Locales[1] {
    s.add(x);
  }
  writeln(s);
}

proc testAddRemoteElementRemote(type T) {
  writeln('Adding remote element ' + T:string + ' remotely');
  var s: set(T);
  on Locales[1] {
    var x = create(T);
    s.add(x);
  }
  writeln(s);
}

proc testAddRemoteElementLocal(type T) {
  writeln('Adding remote element ' + T:string + ' locally');
  var s: set(T);
  on Locales[1] {
    var x = create(T);
    on Locales[0] {
      s.add(x);
    }
  }
  writeln(s);
}

proc testAddLocalElementLocal(type T) {
  writeln('Adding local element ' + T:string + ' locally');
  var s: set(T);
  var x = create(T);
  s.add(x);
  writeln(s);
}

proc testAddToSetLocalAndRemote(type T) {
  testAddLocalElementRemote(T);
  testAddRemoteElementRemote(T);
  testAddRemoteElementLocal(T);
  testAddLocalElementLocal(T);
}

proc test() {
  testAddToSetLocalAndRemote(int); 
  testAddToSetLocalAndRemote(real);
  testAddToSetLocalAndRemote(string);
  testAddToSetLocalAndRemote(bytes);
  testAddToSetLocalAndRemote(r);
  testAddToSetLocalAndRemote(shared C);

  // Use nilable owned to avoid lifetime error complaints.
  testAddToSetLocalAndRemote(owned C?);
}
test();

