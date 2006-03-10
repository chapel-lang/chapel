var i = 12;
var f = 12.0;

function mywrite(x) {
  type select x {
    when integer do writeln("mywrite integer for       ", x);
    when float do   writeln("myrite float for          ", x);
    otherwise       writeln("mywrite not supported for ", x);
  }
}

mywrite(i);
mywrite(f);
mywrite(true);
mywrite("hello world");
