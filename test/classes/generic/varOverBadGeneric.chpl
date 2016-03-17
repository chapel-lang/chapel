config param initInPlace = true;

class C {
  type t;
  var x: t;
}

if initInPlace {
  //
  // This is crammed ridiculously onto one line just to re-use the .good file
  //
  var myC: C = new C();  writeln(myC); } else {  var myC: C;

  myC = new C();

  writeln(myC);
}
