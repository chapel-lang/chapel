/* Documentation for SwitchTypes */
module SwitchTypes {

  record myRec {
    var x: string;
    var y: string;
  }
  class MyClass {
    var x: string;
    var y: string;
  }
  config type myType;
  config param myString1: string;
  config const myString2: string;

  proc main() {
    var t = new myType(myString1, myString2);
    writeln(t.type:string, " ", t);
    if isUnmanagedClass(t) {
      delete t;
    }
  }
}
