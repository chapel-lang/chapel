class c {
  var v: void;
}

var foo = new unmanaged c();
writeln(foo.v);
delete foo;
