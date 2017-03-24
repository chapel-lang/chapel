// submitted by Michael Ferguson 5/18/2011
//
// I'm trying to wrap a class so that it has record semantics;
// where the class is just copied whenever a new version of the
// record is created. But passing the record around causes
// the destructor to be called multiple times.
//
// This kind of thing is easy to do in C++ and used frequently.
// I'd like to do it for GMP support.

class MyClass {
  var x: int;
}

record myrecord {
  var c:MyClass = new MyClass(5);
  proc deinit() { delete c; c = nil; }
}
proc =(ref ret:myrecord, x:myrecord)
{
  delete ret.c;
  ret.c = new MyClass(x.c.x);
}
pragma "init copy fn"
proc chpl__initCopy(x:myrecord)
{
  return new myrecord(new MyClass(x.c.x));
}

var x:myrecord;

writeln(x.c);
// program crashes in this statement.
writeln(x);
