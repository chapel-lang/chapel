class Foo {
  var x: int;
}

class Foo2 {
  var x: bool;
}

operator +(a: Foo, b: Foo) {
  writeln("In DefinesOp.+");
  return new Foo(a.x + b.x);
}

operator align(r: range(?i), count: Foo) {
  writeln("In DefinesOp.align");
  return r align count.x;
}

operator &(lhs: Foo2, rhs: Foo2) {
  writeln("In DefinesOp.&");
  return new Foo2(lhs.x & rhs.x);
}

operator &=(ref lhs: Foo2, rhs: Foo2) {
  writeln("In DefinesOp.&=");
  lhs.x &= rhs.x;
}

operator by(r: range(?i), count: Foo) {
  writeln("In DefinesOp.by");
  return r by count.x;
}

operator :(rhs: Foo, type t: Bar) {
  writeln("In DefinesOp.:");
  return new Bar(rhs.x);
}

operator #(r:range(?i), count: Foo) {
  writeln("In DefinesOp.#");
  return r # count.x;
}

operator /=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp./=");
  lhs.x /= rhs.x;
}

operator /(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp./");
  return new Foo(lhs.x / rhs.x);
}

operator ==(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.==");
  return(lhs.x == rhs.x);
}

operator **=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.**=");
  lhs.x **= rhs.x;
}

operator **(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.**");
  return new Foo(lhs.x ** rhs.x);
}

operator >(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.>");
  return(lhs.x > rhs.x);
}

operator >=(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.>=");
  return(lhs.x >= rhs.x);
}

operator !=(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.!=");
  return(lhs.x != rhs.x);
}

operator <(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.<");
  return(lhs.x < rhs.x);
}

operator <=(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.<=");
  return(lhs.x <= rhs.x);
}

operator -=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.-=");
  lhs.x -= rhs.x;
}

operator %(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.%");
  return new Foo(lhs.x % rhs.x);
}

operator %=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.%=");
  lhs.x %= rhs.x;
}

operator *(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.*");
  return new Foo(lhs.x * rhs.x);
}

operator -(arg: Foo) {
  writeln("In DefinesOp.-");
  return new Foo(-arg.x);
}

operator !(arg: Foo2) {
  writeln("In DefinesOp.!");
  return new Foo2(!arg.x);
}

operator |(lhs: Foo2, rhs: Foo2) {
  writeln("In DefinesOp.|");
  return new Foo2(lhs.x | rhs.x);
}

operator |=(ref lhs: Foo2, rhs: Foo2) {
  writeln("In DefinesOp.|=");
  lhs.x |= rhs.x;
}

operator +=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.+=");
  lhs.x += rhs.x;
}

operator +(arg: Foo) {
  writeln("In DefinesOp.+");
  return new Foo(+arg.x);
}

operator <<(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.<<");
  return new Foo(lhs.x << rhs.x);
}

operator <<=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.<<=");
  lhs.x <<= rhs.x;
}

operator >>(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.>>");
  return new Foo(lhs.x >> rhs.x);
}

operator >>=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.>>=");
  lhs.x >>= rhs.x;
}

operator -(lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.-");
  return new Foo(lhs.x - rhs.x);
}

operator <=>(ref lhs: owned Foo?, ref rhs: owned Foo?) {
  writeln("In DefinesOp.<=>");
  var tmp = lhs;
  lhs = rhs;
  rhs = tmp;
}

operator *=(ref lhs: Foo, rhs: Foo) {
  writeln("In DefinesOp.*=");
  lhs.x *= rhs.x;
}

operator ~(arg: Foo) {
  writeln("In DefinesOp.~");
  return new Foo(~arg.x);
}

operator ^(lhs: Foo2, rhs: Foo2) {
  writeln("In DefinesOp.^");
  return new Foo2(lhs.x ^ rhs.x);
}

operator ^=(ref lhs: Foo2, rhs: Foo2) {
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

operator =(ref lhs: Bar, rhs: Bar) {
  writeln("In DefinesOp.=");
  lhs.y = rhs.y;
}
