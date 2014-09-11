// This test returns an array from a 'var' function.
// Currently it works if accessorFun is either non-'var'
// (which does not change the semantics) or returns a global array.

proc accessorFun() ref {
  var anArray: [1..2] int;
  return anArray;
}

var anotherVar = accessorFun();
writeln(anotherVar);
