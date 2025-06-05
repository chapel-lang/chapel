class Outer {
  param flag;

  // this def is problematic:
  var partialGenericField: Inner(flag);

  // the following works:
  //var partialGenericField: Inner(flag,?);
  //var partialGenericField: Inner;

  proc init(param flag, val) {
    this.flag = flag;
    this.partialGenericField = new Inner(flag, val);
  }
}

class Inner {
  param flag;
  var fullyGenericField;

  proc init(param flag, val) {
    this.flag = flag;
    this.fullyGenericField = val;
  }
}

var o1 = new Outer(false, 5);
// now we've instantiated Inner(bool, int) and Outer(bool, Inner(bool, int))


// here the compiler falsely expects the `partialGenericField` of Outer
// to be Inner(bool,int)
var o2 = new Outer(false, "five");

writeln(o1);
writeln(o2);
