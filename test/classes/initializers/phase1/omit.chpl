class OmitOne {
  var fieldA: bool;
  var fieldB: real;

  proc init(bVal: real) {
    fieldB = bVal;
  }
}

class OmitMultiple {
  var fieldA = 45;
  var fieldB: bool;
  var fieldC: real = -8.4;

  proc init(bVal: bool) {
    fieldB = bVal;
  }
}

proc main() {
  var one = new owned OmitOne(17.9);

  var two = new owned OmitMultiple(true);

  writeln(one);
  writeln(two);
}
