record myShared {
  proc init() {}
  proc init=(s: myShared) {}
  @deprecated("converting myOwned to myShared is deprecated")
  proc init=(o: myOwned) {}
  operator :(x: myOwned, type t: myShared) {
    return new myShared();
  }
}
record myOwned {
  proc init() {}
}

var o = new myOwned();
record R {
  var s: myShared = o;
}

proc main() {
  var r = new R();
}
