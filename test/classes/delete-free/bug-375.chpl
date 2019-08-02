class Parent { }
class Child : Parent { }

proc main() {
  var o = new Child(); // owned Child
  var p = o:shared Parent; // expecting error here
  var q = p: shared Child;
  writeln(q.type:string);
}
