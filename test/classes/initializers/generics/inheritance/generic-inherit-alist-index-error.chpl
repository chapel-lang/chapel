class ClassA {
  type eltType;
  proc init(type eltType) {
    this.eltType = eltType;
    super.init();
  }
}

pragma "use default init"
class ClassB: ClassA {}

proc foo(): ClassB(int) {
  return new ClassB(int);
}

var x = foo();
writeln(x);
delete x;

