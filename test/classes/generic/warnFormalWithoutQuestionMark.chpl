class G {
  type t;
}

class C {
  var x: int;
}

proc foo(x: G) {}                // should warn
proc foo2(x: G(?)) {}

proc bar(x: C) {}

proc baz(xxxyz: shared G) {}     // should warn
proc baz2(x: shared G(?)) {}
proc baz(x: borrowed G) {}       // should warn
proc baz2(x: borrowed G(?)) {}
proc baz(x: owned G) {}          // should warn
proc baz2(x: owned G(?)) {}
proc baz(x: unmanaged G) {}      // should warn
proc baz2(x: unmanaged G(?)) {}

proc bak(x: shared C) {}
proc bak(x: owned C) {}
proc bak(x: borrowed C) {}
proc bak(x: unmanaged C) {}
