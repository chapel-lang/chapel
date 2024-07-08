// Validate the default initializer created for a generic class with no
// constructor or initializer defined
class GenericClass {
  type t;
  param p = 3;
  var v;
  var nongeneric: int;
}

proc main() {
  var ownC1 = new owned GenericClass(bool, 5, 3.0, 2);
  var c1 = ownC1.borrow();
  writeln(c1.type:string);
  writeln(c1);

  var c2: borrowed GenericClass(int, 2, bool)?;
  writeln(c2.type:string);

  var ownC3 = new owned GenericClass(real, 5, 2.4, 8);
  var c3: borrowed GenericClass(real, 5, real) = ownC3.borrow();
  writeln(c3.type:string);
  writeln(c3);
}
