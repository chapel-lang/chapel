var i = 12;
var f = 12.0;

proc mywrite(x) {
  select x.type {
    when int do writeln("mywrite int for       ", x);
    when real do   writeln("myrite real for          ", x);
    otherwise       writeln("mywrite not supported for ", x);
  }
}

mywrite(i);
mywrite(f);
mywrite(true);
mywrite("hello world");
