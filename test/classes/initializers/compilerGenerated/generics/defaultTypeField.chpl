
//
// Test to ensure that we can resolve default initializers for types with a
// type field that has a default value.
//

pragma "use default init"
class Parent {
  type idxType;
}

pragma "use default init"
class Child : Parent {
  type foo = 2*idxType;
  var x : foo;
}

var c : Child(int);
writeln("c.type = ", c.type:string);
