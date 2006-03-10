var i = 12;
var f = 12.0;

function mywrite(x) {
  function bar(tmp:integer) {
    writeln("mywrite integer for       ", x);
  }
  function bar(tmp:integer) {
    writeln("myrite float for          ", x);
  }
  function bar(tmp:integer) {
    writeln("mywrite not supported for ", x);
  }
  bar(x);
}

mywrite(i);
mywrite(f);
mywrite(true);
mywrite("hello world");
