def foo(param x: int) {
  writeln("Got into param foo with value ", x);
}

def foo(x: int) {
  writeln("Got into non-param foo with value ", x);
}

def param string.bar param return 3;

def string.bar param return 4;

def string.baz return 5;

foo("test".bar);
foo("test".baz);

config const which = true;

var mystring = if which then "hi" else "bye";

foo(mystring.bar);
foo(mystring.baz);
