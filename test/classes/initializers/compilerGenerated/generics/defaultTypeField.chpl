
//
// Test to ensure that we can resolve default initializers for types with a
// type field that has a default value.
//

class Parent {
  type idxType;
}

class Child : Parent {
  type foo = 2*idxType;
  var x : foo;
}

var c : borrowed Child(int)?;
writeln("c.type = ", c.type:string);
