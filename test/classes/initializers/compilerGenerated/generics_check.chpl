// Validate the default initializer created for a generic class with no
// constructor or initializer defined
class GenericClass {
  type t;
  param p = 3;
  var v;
  var nongeneric: int;
}

proc main() {
  var c1 = new GenericClass(bool, 5, 3.0, 2);
  writeln(c1.type:string);
  writeln(c1);
  delete c1;

  var c2: GenericClass(int, 2, bool);
  writeln(c2.type:string);

  var c3: GenericClass(real, 5, real) = new GenericClass(real, 5, 2.4, 8);
  writeln(c3.type:string);
  writeln(c3);
  delete c3;
}
