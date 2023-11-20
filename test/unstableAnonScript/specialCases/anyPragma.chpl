// Copied from test/unstable/anyPragma.chpl
// place some pragmas on various symbols to make sure we get
// unstable warnings from them

pragma "auto ii"
module M {
  pragma "config"
  pragma "aggregation marker"
  const x : int;

  pragma "allow ref"
  enum Color {red, blue, green};

  pragma "static only aggregation marker"
  var y : real;

  pragma "static and dynamic aggregation marker"
  pragma "aggregator generator"
  pragma "aliasing array"
  record R {

    pragma "always RVF"
    var a: string;

    pragma "const"
    proc bar() {

    }
  }

  pragma "build tuple"
  pragma "build tuple type"
  var myTuple = (1, "two");

  pragma "get file name"
  type chroma = int;


  var actual = 10;
  pragma "ref if modified"
  ref refToActual = actual;

  pragma "alias scope from this"
  pragma "ignore transfer errors"
  pragma "find user line"
  pragma "user variable name"
  proc foo() {
    writeln("in foo");
  }

}
