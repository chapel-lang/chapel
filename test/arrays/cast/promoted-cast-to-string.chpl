record R {
  var x: int;

  operator: (r: R, type t:string) {
    return "-" + r.x:string + "-";
  }
}

proc string.init=(r: R) {
  init this;
  this = r:string;
}

const d = {1..10},
      a = [i in d] new R(i),
      b: [d] string = a;

writeln(b);
