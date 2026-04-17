/*
  Classes and records should define all fields before methods.
*/

// This class mixes fields and methods which violates the rule.
class BadClass {
  var name: string;
  proc printName() {
    writeln(name);
  }
  var age: int;
}

// This class defines all fields before methods which is correct.
class GoodClass {
  var name: string;
  var age: int;
  proc printName() {
    writeln(name);
  }
}
