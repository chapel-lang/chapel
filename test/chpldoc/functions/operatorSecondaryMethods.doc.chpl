class Foo {
  var x: int;
}

class Foo2 {
  var x: bool;
}

operator Foo.+(a: Foo, b: Foo) {
  writeln("In DefinesOp.+");
  return new Foo(a.x + b.x);
}

operator Foo.align(r: range(?i), count: Foo) {
  writeln("In DefinesOp.align");
  return r align count.x;
}

operator Foo2.&(lhs: Foo2, rhs: Foo2) {
  writeln("In DefinesOp.&");
  return new Foo2(lhs.x & rhs.x);
}

operator Foo2.&=(ref lhs: Foo2, rhs: Foo2) {
  writeln("In DefinesOp.&=");
  lhs.x &= rhs.x;
}

operator Foo.by(r: range(?i), count: Foo) {
  writeln("In DefinesOp.by");
  return r by count.x;
}

operator Foo.:(rhs: Foo, type t: Bar) {
  writeln("In DefinesOp.:");
  return new Bar(rhs.x);
}

operator Foo.#(r:range(?i), count: Foo) {
  writeln("In DefinesOp.#");
  return r # count.x;
}

operator Foo./=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp./=");
  lhs.x /= rhs.x;
}

operator Foo./(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp./");
  return new Foo(lhs.x / rhs.x);
}

operator Foo.==(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.==");
  return(lhs.x == rhs.x);
}

operator Foo.**=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.**=");
  lhs.x **= rhs.x;
}

operator Foo.**(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.**");
  return new Foo(lhs.x ** rhs.x);
}

operator Foo.>(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.>");
  return(lhs.x > rhs.x);
}

operator Foo.>=(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.>=");
  return(lhs.x >= rhs.x);
}

operator Foo.!=(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.!=");
  return(lhs.x != rhs.x);
}

use IO;

operator Foo.<~>(const ref ch: channel, const x: Foo) const ref throws
  where ch.writing {

  writeln("In DefinesOp.<~>");
  ch.write(x.x);
  return ch;
}

operator Foo.<(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.<");
  return(lhs.x < rhs.x);
}

operator Foo.<=(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.<=");
  return(lhs.x <= rhs.x);
}

operator Foo.-=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.-=");
  lhs.x -= rhs.x;
}

operator Foo.%(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.%");
  return new Foo(lhs.x % rhs.x);
}

operator Foo.%=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.%=");
  lhs.x %= rhs.x;
}

operator Foo.*(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.*");
  return new Foo(lhs.x * rhs.x);
}

operator Foo.-(arg: Foo) {
  writeln("In DefinesOp.-");
  return new Foo(-arg.x);
}

operator Foo2.!(arg: Foo2) {
  writeln("In DefinesOp.!");
  return new Foo2(!arg.x);
}

operator Foo2.|(lhs: Foo2, rhs: Foo2) {
  writeln("In DefinesOp.|");
  return new Foo2(lhs.x | rhs.x);
}

operator Foo2.|=(ref lhs: Foo2, rhs: Foo2) {
  writeln("In DefinesOp.|=");
  lhs.x |= rhs.x;
}

operator Foo.+=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.+=");
  lhs.x += rhs.x;
}

operator Foo.+(arg: Foo) {
  writeln("In DefinesOp.+");
  return new Foo(+arg.x);
}

operator Foo.<<(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.<<");
  return new Foo(lhs.x << rhs.x);
}

operator Foo.<<=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.<<=");
  lhs.x <<= rhs.x;
}

operator Foo.>>(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.>>");
  return new Foo(lhs.x >> rhs.x);
}

operator Foo.>>=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.>>=");
  lhs.x >>= rhs.x;
}

operator Foo.-(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.-");
  return new Foo(lhs.x - rhs.x);
}

operator Foo.<=>(ref lhs: owned Foo?, ref rhs: owned Foo?) {
  writeln("In DefinesOp.<=>");
  var tmp = lhs;
  lhs = rhs;
  rhs = tmp;
}

operator Foo.*=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.*=");
  lhs.x *= rhs.x;
}

operator Foo.~(arg: Foo) {
  writeln("In DefinesOp.~");
  return new Foo(~arg.x);
}

operator Foo2.^(lhs: Foo2, rhs: Foo2) {
  writeln("In DefinesOp.^");
  return new Foo2(lhs.x ^ rhs.x);
}

operator Foo2.^=(ref lhs: Foo2, rhs: Foo2) {
  writeln("In DefinesOp.^=");
  lhs.x ^= rhs.x;
}

record Bar {
  var y: int;
}

proc Bar.init=(other: Bar) {
  writeln("In init=");
  this.y = other.y;
}

operator Bar.=(ref lhs: Bar, rhs: Bar) {
  writeln("In DefinesOp.=");
  lhs.y = rhs.y;
}
