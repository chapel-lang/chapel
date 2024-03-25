// see issue #24593
// classes have the same issue
// can compile with --minimal-modules upon removal of writeln()

record SymEntry1 {
  var a: real;
  proc init(a) do this.a = a;
}

var se1 = new SymEntry1(5);    // success, a = 5.0
writeln(se1);

record SymEntry2 {
  var a = makeDistArray();
  proc init(a) do this.a = a;  // error: ~"cannot assign int to real"
                               // same error if we wrote 'init(a:int)'
}

proc makeDistArray() do return 3.4;

var se2 = new SymEntry2(5);    // causes the above error
writeln(se2);
