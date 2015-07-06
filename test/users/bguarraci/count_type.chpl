// #count doesn't preserve type of count
class Foo {
  var count: uint;
  var buffer: [0..#count] int;
}

var foo = new Foo(3);

// false
writeln(foo.count.type == foo.buffer.domain.high.type);

// for-loop domain that uses count produces an i type that's not compatible with buffer's domain
for i in 0..foo.count-1 {
  // count_type.chpl:22: error: unresolved access of '[domain(1,int(64),false)] int(64)' by '[uint(64)]'
  writeln(i.type == foo.buffer.domain.high.type);
  // writeln(foo.buffer[i]);
}
