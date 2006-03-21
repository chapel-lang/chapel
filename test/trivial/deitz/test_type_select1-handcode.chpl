var i = 12;
var f = 12.0;

fun mywrite(x) {
  fun bar(tmp:int) {
    writeln("mywrite int for       ", x);
  }
  fun bar(tmp:float) {
    writeln("myrite float for          ", x);
  }
  fun bar(tmp) {
    writeln("mywrite not supported for ", x);
  }
  bar(x);
}

mywrite(i);
mywrite(f);
mywrite(true);
mywrite("hello world");
