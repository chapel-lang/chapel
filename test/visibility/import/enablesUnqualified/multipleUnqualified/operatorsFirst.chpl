module DefinesOp {
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

  use IO;

  operator <~>(const ref ch: channel, const x: Foo) const ref throws
    where ch.writing {

    writeln("In DefinesOp.<~>");
    try ch.write(x.x);
    return ch;
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
}

// Checks when the operator is the first symbol listed in the `import`
// limitation clause
proc main() {
  {
    import this.DefinesOp.{+, Foo};
    // Note: the above also brings in the unary + operator

    var a = new Foo(3);
    var b = new Foo(7);
    writeln(a + b);
  }

  {
    import this.DefinesOp.{align, Foo};

    var r1 = 0..6;
    var a = new Foo(3);
    var r1b = r1 by 4 align a;
    writeln(r1b);
    for i in r1b {
      writeln(i);
    }
  }

  {
    import this.DefinesOp.{&, Foo2};

    var a = new Foo2(true);
    var b = new Foo2(false);
    writeln(a & b);
  }

  {
    import this.DefinesOp.{&=, Foo2};

    var a = new Foo2(true);
    var b = new Foo2(false);
    a &= b;
    writeln(a);
  }

  {
    import this.DefinesOp.{by, Foo};

    var r1 = 0..6;
    var foo = new Foo(3);
    writeln(r1 by foo);
  }

  {
    import this.DefinesOp.{:, Foo, Bar};

    var foo = new Foo(3);
    var bar = foo: Bar;
    writeln(bar.type: string);
    writeln(bar);
  }

  {
    import this.DefinesOp.{#, Foo};

    var r1 = 0..6;
    var foo = new Foo(3);
    writeln(r1 # foo);
  }

  {
    import this.DefinesOp.{/=, Foo};

    var a = new Foo(6);
    var b = new Foo(3);
    a /= b;
    writeln(a);
  }

  {
    import this.DefinesOp.{/, Foo};

    var a = new Foo(6);
    var b = new Foo(3);
    writeln(a / b);
  }

  {
    import this.DefinesOp.{==, Foo};

    var a = new Foo(6);
    var b = new Foo(3);
    writeln(a == b);
  }

  {
    import this.DefinesOp.{**=, Foo};

    var a = new Foo(6);
    var b = new Foo(3);
    a **= b;
    writeln(a);
  }

  {
    import this.DefinesOp.{**, Foo};

    var a = new Foo(6);
    var b = new Foo(3);
    writeln(a ** b);
  }

  {
    import this.DefinesOp.{>, Foo};

    var a = new Foo(6);
    var b = new Foo(3);
    writeln(a > b);
  }

  {
    import this.DefinesOp.{>=, Foo};

    var a = new Foo(6);
    var b = new Foo(3);
    writeln(a >= b);
  }

  {
    import this.DefinesOp.{!=, Foo};

    var a = new Foo(6);
    var b = new Foo(3);
    writeln(a != b);
  }

  {
    use IO only <~>, ioNewline, stdout;
    import this.DefinesOp.{<~>, Foo};

    var foo = new Foo(3);
    stdout <~> foo;
    stdout <~> new ioNewline();
  }

  {
    import this.DefinesOp.{<, Foo};

    var a = new Foo(3);
    var b = new Foo(6);
    writeln(a < b);
  }

  {
    import this.DefinesOp.{<=, Foo};

    var a = new Foo(3);
    var b = new Foo(6);
    writeln(a <= b);
  }

  {
    import this.DefinesOp.{-=, Foo};

    var a = new Foo(3);
    var b = new Foo(6);
    a -= b;
    writeln(a);
  }

  {
    import this.DefinesOp.{%, Foo};

    var a = new Foo(6);
    var b = new Foo(5);
    writeln(a % b);
  }

  {
    import this.DefinesOp.{%=, Foo};

    var a = new Foo(6);
    var b = new Foo(5);
    a %= b;
    writeln(a);
  }

  {
    import this.DefinesOp.{*, Foo};

    var a = new Foo(6);
    var b = new Foo(5);
    writeln(a * b);
  }

  {
    import this.DefinesOp.{-, Foo};
    // Note: the above also brings in the binary - operator

    var a = new Foo(3);
    writeln(-a);
  }

  {
    import this.DefinesOp.{!, Foo2};

    var a = new Foo2(true);
    writeln(!a);
  }

  {
    import this.DefinesOp.{|, Foo2};

    var a = new Foo2(true);
    var b = new Foo2(false);
    writeln(a | b);
  }

  {
    import this.DefinesOp.{|=, Foo2};

    var a = new Foo2(true);
    var b = new Foo2(false);
    a |= b;
    writeln(a);
  }

  {
    import this.DefinesOp.{+=, Foo};

    var a = new Foo(2);
    var b = new Foo(3);
    a += b;
    writeln(a);
  }

  {
    import this.DefinesOp.{+, Foo};
    // Note: also brings in binary addition operator

    var a = new Foo(-2);
    writeln(+a);
  }

  {
    import this.DefinesOp.{<<, Foo};

    var a = new Foo(6);
    var b = new Foo(2);
    writeln(a << b);
  }

  {
    import this.DefinesOp.{<<=, Foo};

    var a = new Foo(6);
    var b = new Foo(2);
    a <<= b;
    writeln(a);
  }

  {
    import this.DefinesOp.{>>, Foo};

    var a = new Foo(6);
    var b = new Foo(2);
    writeln(a >> b);
  }

  {
    import this.DefinesOp.{>>=, Foo};

    var a = new Foo(6);
    var b = new Foo(2);
    a >>= b;
    writeln(a);
  }

  {
    import this.DefinesOp.{-, Foo};
    // Note: also brings in the unary negation operator

    var a = new Foo(6);
    var b = new Foo(2);
    writeln(a - b);
  }

  {
    import this.DefinesOp.{<=>, Foo};

    var a: owned Foo? = new Foo(6);
    var b: owned Foo? = new Foo(2);
    // Note: must be explicit with type of a and b due to nilability and
    // potential conflicts with the owned swap operator
    a <=> b;
    writeln(a);
    writeln(b);
  }

  {
    import this.DefinesOp.{*=, Foo};

    var a = new Foo(2);
    var b = new Foo(3);
    a *= b;
    writeln(a);
  }

  {
    import this.DefinesOp.{~, Foo};

    var a = new Foo(3);
    writeln(~a);
  }

  {
    import this.DefinesOp.{^, Foo2};

    var a = new Foo2(true);
    var b = new Foo2(false);
    writeln(a ^ b);
  }

  {
    import this.DefinesOp.{^=, Foo2};

    var a = new Foo2(true);
    var b = new Foo2(false);
    a ^= b;
    writeln(a);
  }

  {
    import this.DefinesOp.{=, Bar};

    var a = new Bar(3);
    var b = new Bar(5);
    a = b;
    writeln(a);
  }
}
