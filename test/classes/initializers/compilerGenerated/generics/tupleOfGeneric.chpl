
record R {
  type idxType;
  param stridable : bool;
  var low, high : idxType;

  proc init(type idxType = int,
            param stridable : bool = false) {
    this.idxType   = idxType;
    this.stridable = stridable;
  }
}

var r : R(int(32), false);
writeln("r = ", r);

//
// The tricky part here is that _defaultOf is generated **and** normalized
// in the middle of resolution. The type of the tuple field "x1" is known
// at this point, which lead the compiler to think that the field was not a
// generic record.
//
// Clearly the type is concrete, but the fact that it was instantiated from
// a generic has consequences for the way 'init' is called.
//
// At this time this test was created, the compiler was inserting a zero-args
// init call, which used the default 'int(64)' type for 'idxType'. This would
// eventually lead to a type mismatch failure later in resolution.
//
var t : 1*R(int(32), false);
writeln("t = ", t);
