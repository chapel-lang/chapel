module ProvidesTypes {
  class Foo {
    var x: int;
  }

  class Foo2 {
    var x: bool;
  }

  record Bar {
    var y: int;
  }
}

module ProvidesOps {
  use ProvidesTypes;

  operator +(a: Foo, b: Foo) {
    writeln("In ProvidesOps.+");
    return new Foo(a.x + b.x);
  }

  operator align(r: range(?i), count: Foo) {
    writeln("In ProvidesOps.align");
    return r align count.x;
  }

  operator &(lhs: Foo2, rhs: Foo2) {
    writeln("In ProvidesOps.&");
    return new Foo2(lhs.x & rhs.x);
  }

  operator &=(ref lhs: Foo2, rhs: Foo2) {
    writeln("In ProvidesOps.&=");
    lhs.x &= rhs.x;
  }

  operator by(r: range(?i), count: Foo) {
    writeln("In ProvidesOps.by");
    return r by count.x;
  }

  operator :(rhs: Foo, type t: Bar) {
    writeln("In ProvidesOps.:");
    return new Bar(rhs.x);
  }

  operator #(r:range(?i), count: Foo) {
    writeln("In ProvidesOps.#");
    return r # count.x;
  }

  operator /=(ref lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps./=");
    lhs.x /= rhs.x;
  }

  operator /(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps./");
    return new Foo(lhs.x / rhs.x);
  }

  operator ==(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.==");
    return(lhs.x == rhs.x);
  }

  operator **=(ref lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.**=");
    lhs.x **= rhs.x;
  }

  operator **(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.**");
    return new Foo(lhs.x ** rhs.x);
  }

  operator >(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.>");
    return(lhs.x > rhs.x);
  }

  operator >=(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.>=");
    return(lhs.x >= rhs.x);
  }

  operator !=(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.!=");
    return(lhs.x != rhs.x);
  }

  use IO;

  operator <~>(const ref ch: channel, const x: Foo) const ref throws
    where ch.writing {

    writeln("In ProvidesOps.<~>");
    try ch.readwrite(x.x);
    return ch;
  }

  operator <(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.<");
    return(lhs.x < rhs.x);
  }

  operator <=(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.<=");
    return(lhs.x <= rhs.x);
  }

  operator -=(ref lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.-=");
    lhs.x -= rhs.x;
  }

  operator %(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.%");
    return new Foo(lhs.x % rhs.x);
  }

  operator %=(ref lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.%=");
    lhs.x %= rhs.x;
  }

  operator *(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.*");
    return new Foo(lhs.x * rhs.x);
  }

  operator -(arg: Foo) {
    writeln("In ProvidesOps.-");
    return new Foo(-arg.x);
  }

  operator !(arg: Foo2) {
    writeln("In ProvidesOps.!");
    return new Foo2(!arg.x);
  }

  operator |(lhs: Foo2, rhs: Foo2) {
    writeln("In ProvidesOps.|");
    return new Foo2(lhs.x | rhs.x);
  }

  operator |=(ref lhs: Foo2, rhs: Foo2) {
    writeln("In ProvidesOps.|=");
    lhs.x |= rhs.x;
  }

  operator +=(ref lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.+=");
    lhs.x += rhs.x;
  }

  operator +(arg: Foo) {
    writeln("In ProvidesOps.+");
    return new Foo(+arg.x);
  }

  operator <<(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.<<");
    return new Foo(lhs.x << rhs.x);
  }

  operator <<=(ref lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.<<=");
    lhs.x <<= rhs.x;
  }

  operator >>(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.>>");
    return new Foo(lhs.x >> rhs.x);
  }

  operator >>=(ref lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.>>=");
    lhs.x >>= rhs.x;
  }

  operator -(lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.-");
    return new Foo(lhs.x - rhs.x);
  }

  operator <=>(ref lhs: owned Foo?, ref rhs: owned Foo?) {
    writeln("In ProvidesOps.<=>");
    var tmp = lhs;
    lhs = rhs;
    rhs = tmp;
  }

  operator *=(ref lhs: Foo, rhs: Foo) {
    writeln("In ProvidesOps.*=");
    lhs.x *= rhs.x;
  }

  operator ~(arg: Foo) {
    writeln("In ProvidesOps.~");
    return new Foo(~arg.x);
  }

  operator ^(lhs: Foo2, rhs: Foo2) {
    writeln("In ProvidesOps.^");
    return new Foo2(lhs.x ^ rhs.x);
  }

  operator ^=(ref lhs: Foo2, rhs: Foo2) {
    writeln("In ProvidesOps.^=");
    lhs.x ^= rhs.x;
  }
}
