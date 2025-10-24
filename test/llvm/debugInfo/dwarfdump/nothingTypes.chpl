use Debugger;

record myRecord {
  type t1;
  type t2;
  var x: t1;
  var y: t2;
  proc init(type t1, type t2) {
    this.t1 = t1;
    this.t2 = t2;
  }
}
class MyClass {
  type t1;
  type t2;
  var x: t1;
  var y: t2;
  proc init(type t1, type t2) {
    this.t1 = t1;
    this.t2 = t2;
  }
}

proc test(type t) {
  // DWARFDUMP: myVar1
  // DWARFDUMP: myRecord(int(64),real(64))
  // DWARFDUMP: MyClass(int(64),real(64))
  var myVar1 = new t(int, real);
  // DWARFDUMP: myVar2
  // DWARFDUMP: myRecord(nothing,nothing)
  // DWARFDUMP: MyClass(nothing,nothing)
  var myVar2 = new t(nothing, nothing);
  // DWARFDUMP: myVar3
  // DWARFDUMP: myRecord(int(64),nothing)
  // DWARFDUMP: MyClass(int(64),nothing)
  var myVar3 = new t(int, nothing);
  // DWARFDUMP: myVar4
  // DWARFDUMP: myRecord(nothing,real(64))
  // DWARFDUMP: myRecord(myRecord(nothing,real(64)),int(64))
  // DWARFDUMP: MyClass(nothing,real(64))
  // DWARFDUMP: MyClass(unmanaged MyClass(nothing,real(64))?,int(64))
  var myVar4 = new t(t(nothing, real), int);

  writeln(myVar1, myVar2, myVar3, myVar4, sep=" | ");
  breakpoint;

  if isUnmanagedClassType(t) {
    delete myVar1;
    delete myVar2;
    delete myVar3;
    delete myVar4;
  }
}
proc main() {
  test(myRecord(?));
  test(unmanaged MyClass(?)?);
  // doesn't work due to bug with generic nilable managed class types
  // test(shared MyClass(?)?);
}
