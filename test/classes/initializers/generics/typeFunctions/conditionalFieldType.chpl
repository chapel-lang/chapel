
proc helper(param cond : bool, type t) type {
  if cond {
    return t;
  } else {
    return nothing;
  }
}

record R {
  type idxType;
  param stridable : bool;

  var stride : helper(stridable, idxType);

  proc init(type idxType,
            param stridable : bool,
            stride : int = 1) {
    this.idxType = idxType;
    this.stridable = stridable;
    this.complete();
    if stridable then this.stride = stride;
  }
}

class C {
  type idxType;
  param stridable : bool;

  var stride : helper(stridable, idxType);

  proc init(type idxType,
            param stridable : bool,
            stride : int = 1) {
    this.idxType = idxType;
    this.stridable = stridable;
    this.complete();
    if stridable then this.stride = stride;
  }
}

writeln("----- Records -----");
var rX = new R(int, false);
writeln("rX = ", rX);
writeln("rX.stride.type = ", rX.stride.type:string);

var rY = new R(int, true, 5);
writeln("rY = ", rY);
writeln("rY.stride.type = ", rY.stride.type:string);

writeln();
writeln("----- Classes -----");
var cX = new R(int, false);
writeln("cX = ", cX);
writeln("cX.stride.type = ", cX.stride.type:string);

var cY = new R(int, true, 5);
writeln("cY = ", cY);
writeln("cY.stride.type = ", cY.stride.type:string);
