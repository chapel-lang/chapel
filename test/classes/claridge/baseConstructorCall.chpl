class BaseClass {
  var a, b: int;

  def BaseClass(b: int) {
    this.b = b;
  }
}

class DerivedClass: BaseClass {
  var c: int;

  def DerivedClass(b:int, c:int) {
    this.b = b;
    this.c = c;
  }
}

var D = new DerivedClass(1,2);
writeln(D);