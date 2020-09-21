record R {
  var s:string;
  proc deinit() {
    writeln("deinit ", s);
  }
}

config const option = true;

var a;
var b;

b = new R("b");
a = new R("a");

var c;
var d;

{
  d = new R("d");
  c = new R("c");
}

var e;
var f;

if option {
  f = new R("f");
  e = new R("e");
} else {
  f = new R("f");
  e = new R("e");
}
