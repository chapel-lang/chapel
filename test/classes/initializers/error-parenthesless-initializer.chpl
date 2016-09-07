// Modified from
// test/classes/constructors/error-parenthesless-constructor.chpl
class x {
  var v:int;
  proc init { writeln("x initializer"); }
}

var z = new x();
