record R {
  var s:string;
  proc deinit() {
    writeln("deinit ", s);
  }
}

config const option = true;

var a;
var b;

if option {
  b = new R("b");
  a = new R("a");
} else {
  a = new R("a");
  b = new R("b");
}
