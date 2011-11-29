var i = 12;
var f = 12.0;

proc mywrite(x) {
  proc bar(tmp:int) {
    writeln("mywrite int for       ", x);
  }
  proc bar(tmp:real) {
    writeln("myrite real for          ", x);
  }
  proc bar(tmp) {
    writeln("mywrite not supported for ", x);
  }
  bar(x);
}

mywrite(i);
mywrite(f);
mywrite(true);
mywrite("hello world");
