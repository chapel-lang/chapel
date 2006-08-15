var i = 12;
var f = 12.0;

def mywrite(x) {
  def bar(tmp:int) {
    writeln("mywrite int for       ", x);
  }
  def bar(tmp:float) {
    writeln("myrite float for          ", x);
  }
  def bar(tmp) {
    writeln("mywrite not supported for ", x);
  }
  bar(x);
}

mywrite(i);
mywrite(f);
mywrite(true);
mywrite("hello world");
