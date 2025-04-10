use Python;

proc main() {
  var interp = new Interpreter();

  doOps(new Value(interp, 1), new Value(interp, 2));
  doOps(new Value(interp, 1.0), new Value(interp, 2.0));
  doOps(new Value(interp, "hello"), new Value(interp, "world"));
}

proc doOps(in lhs, in rhs) throws {
  writeln("lhs: ", lhs);
  writeln("rhs: ", rhs);

  writeln(" binary ops:");
  {
    var a = lhs + rhs;
    writeln("  +: ", a, " ", a.type:string);
    var b = lhs - rhs;
    writeln("  -: ", b, " ", b.type:string);
    var c = lhs * rhs;
    writeln("  *: ", c, " ", c.type:string);
    var d = lhs / rhs;
    writeln("  /: ", d, " ", d.type:string);
    var e = lhs % rhs;
    writeln("  %: ", e, " ", e.type:string);
    var f = lhs ** rhs;
    writeln("  **: ", f, " ", f.type:string);
    var g = lhs & rhs;
    writeln("  &: ", g, " ", g.type:string);
    var h = lhs | rhs;
    writeln("  |: ", h, " ", h.type:string);
    var i = lhs ^ rhs;
    writeln("  ^: ", i, " ", i.type:string);
    var j = lhs << rhs;
    writeln("  <<: ", j, " ", j.type:string);
    var k = lhs >> rhs;
    writeln("  >>: ", k, " ", k.type:string);
    var l = lhs == rhs;
    writeln("  ==: ", l, " ", l.type:string);
    var m = lhs != rhs;
    writeln("  !=: ", m, " ", m.type:string);
    var n = lhs < rhs;
    writeln("  <: ", n, " ", n.type:string);
    var o = lhs <= rhs;
    writeln("  <=: ", o, " ", o.type:string);
    var p = lhs > rhs;
    writeln("  >: ", p, " ", p.type:string);
    var q = lhs >= rhs;
    writeln("  >=: ", q, " ", q.type:string);
  }

  writeln(" unary ops:");
  {
    var a = +lhs;
    writeln("  +: ", a, " ", a.type:string);
    var b = -lhs;
    writeln("  -: ", b, " ", b.type:string);
    var c = ~lhs;
    writeln("  ~: ", c, " ", c.type:string);
    var d = !lhs;
    writeln("  !: ", d, " ", d.type:string);
  }

  writeln(" assignment ops:");
  {
    var a = lhs.value(owned Value);
    a += rhs;
    writeln("  +=: ", a, " ", a.type:string);
    var b = lhs.value(owned Value);
    b -= rhs;
    writeln("  -=: ", b, " ", b.type:string);
    var c = lhs.value(owned Value);
    c *= rhs;
    writeln("  *=: ", c, " ", c.type:string);
    var d = lhs.value(owned Value);
    d /= rhs;
    writeln("  /=: ", d, " ", d.type:string);
    var e = lhs.value(owned Value);
    e %= rhs;
    writeln("  %=: ", e, " ", e.type:string);
    var f = lhs.value(owned Value);
    f **= rhs;
    writeln("  **=: ", f, " ", f.type:string);
    var g = lhs.value(owned Value);
    g &= rhs;
    writeln("  &=: ", g, " ", g.type:string);
    var h = lhs.value(owned Value);
    h |= rhs;
    writeln("  |=: ", h, " ", h.type:string);
    var i = lhs.value(owned Value);
    i ^= rhs;
    writeln("  ^=: ", i, " ", i.type:string);
    var j = lhs.value(owned Value);
    j <<= rhs;
    writeln("  <<=: ", j, " ", j.type:string);
    var k = lhs.value(owned Value);
    k >>= rhs;
    writeln("  >>=: ", k, " ", k.type:string);
  }
  writeln("========================");
}
