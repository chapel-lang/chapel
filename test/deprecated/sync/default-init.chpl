// deprecated by Jade in 1.32, should be moved to test/types/sync after deprecation period as compile failures
config param testnum: int = 1;

if testnum == 1 {
  record R {
    var x: sync int;
  }
  var r = new R();
}
else if testnum == 2 {
  class C {
    var x: sync int;
  }
  var c = new C();
}
else if testnum == 3 {
  union U {
    var x: sync int;
  }
  var u = new U();
}
else if testnum == 4 {
  record R {
    var x: [0..1] sync int;
  }
  var r = new R();
}
else if testnum == 5 {
  record R {
    var x: [0..1] ([0..1] sync int);
  }
  var r = new R();
}
// both of these cases already correctly fail to compile
// else if testnum == 6 {
//   record R {
//     var x: (sync int, sync int);
//   }
//   var r = new R();
// }
// else if testnum == 7 {
//   record R {
//     var x: (int, (int, sync int));
//     proc init() {} // no init=, still warn
//   }
//   var r = new R();
// }
else if testnum == 8 {
  record R {
    var x: sync int;
    proc init() {}
    proc init=(other: R) {}
  }
  var r = new R();
  class C {
    var x: sync int;
    proc init() {}
  }
  var c = new C();
  union U {
    var x: sync int;
    proc init() {}
    proc init=(other: U) {}
  }
  var u = new U();
}
