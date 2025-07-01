use Python;

proc main() {
  var interp = new Interpreter();

  doOps(new Value(interp, 1), new Value(interp, 2));
  doOps(new Value(interp, 2.0), new Value(interp, 1.0));
  doOps(new Value(interp, "hello"), new Value(interp, "world"));
  doOps(new Value(interp, false), new Value(interp, true));
}

proc doOps(in lhs, in rhs) throws {
  writeln("lhs: ", lhs);
  writeln("rhs: ", rhs);

  writeln(" binary ops:");
  {
    try {
      var a = lhs + rhs;
      writeln("  +: ", a, " ", a.type:string, " ", a.pyType());
    } catch e {
      writeln("  +: Exception: ", e.message());
    }
    try {
      var b = lhs - rhs;
      writeln("  -: ", b, " ", b.type:string, " ", b.pyType());
    } catch e {
      writeln("  -: Exception: ", e.message());
    }
    try {
      var c = lhs * rhs;
      writeln("  *: ", c, " ", c.type:string, " ", c.pyType());
    } catch e {
      writeln("  *: Exception: ", e.message());
    }
    try {
      var d = lhs / rhs;
      writeln("  /: ", d, " ", d.type:string, " ", d.pyType());
    } catch e {
      writeln("  /: Exception: ", e.message());
    }
    try {
      var e = lhs % rhs;
      writeln("  %: ", e, " ", e.type:string, " ", e.pyType());
    } catch e {
      writeln("  %: Exception: ", e.message());
    }
    try {
      var f = lhs ** rhs;
      writeln("  **: ", f, " ", f.type:string, " ", f.pyType());
    } catch e {
      writeln("  **: Exception: ", e.message());
    }
    try {
      var g = lhs & rhs;
      writeln("  &: ", g, " ", g.type:string, " ", g.pyType());
    } catch e {
      writeln("  &: Exception: ", e.message());
    }
    try {
      var h = lhs | rhs;
      writeln("  |: ", h, " ", h.type:string, " ", h.pyType());
    } catch e {
      writeln("  |: Exception: ", e.message());
    }
    try {
      var i = lhs ^ rhs;
      writeln("  ^: ", i, " ", i.type:string, " ", i.pyType());
    } catch e {
      writeln("  ^: Exception: ", e.message());
    }
    try {
      var j = lhs << rhs;
      writeln("  <<: ", j, " ", j.type:string, " ", j.pyType());
    } catch e {
      writeln("  <<: Exception: ", e.message());
    }
    try {
      var k = lhs >> rhs;
      writeln("  >>: ", k, " ", k.type:string, " ", k.pyType());
    } catch e {
      writeln("  >>: Exception: ", e.message());
    }
  }

  writeln(" comparison ops:");
  {
    try {
      var l = lhs == rhs;
      writeln("  ==: ", l, " ", l.type:string);
    } catch e {
      writeln("  ==: Exception: ", e.message());
    }
    try {
      var m = lhs != rhs;
      writeln("  !=: ", m, " ", m.type:string);
    } catch e {
      writeln("  !=: Exception: ", e.message());
    }
    try {
      var n = lhs < rhs;
      writeln("  <: ", n, " ", n.type:string);
    } catch e {
      writeln("  <: Exception: ", e.message());
    }
    try {
      var o = lhs <= rhs;
      writeln("  <=: ", o, " ", o.type:string);
    } catch e {
      writeln("  <=: Exception: ", e.message());
    }
    try {
      var p = lhs > rhs;
      writeln("  >: ", p, " ", p.type:string);
    } catch e {
      writeln("  >: Exception: ", e.message());
    }
    try {
      var q = lhs >= rhs;
      writeln("  >=: ", q, " ", q.type:string);
    } catch e {
      writeln("  >=: Exception: ", e.message());
    }
  }

  writeln(" unary ops:");
  {
    try {
      var a = +lhs;
      writeln("  +: ", a, " ", a.type:string, " ", a.pyType());
    } catch e {
      writeln("  +: Exception: ", e.message());
    }
    try {
      var b = -lhs;
      writeln("  -: ", b, " ", b.type:string, " ", b.pyType());
    } catch e {
      writeln("  -: Exception: ", e.message());
    }
    try {
      var c = ~lhs;
      writeln("  ~: ", c, " ", c.type:string, " ", c.pyType());
    } catch e {
      writeln("  ~: Exception: ", e.message());
    }
    try {
      var d = !lhs;
      writeln("  !: ", d, " ", d.type:string, " ", d.pyType());
    } catch e {
      writeln("  !: Exception: ", e.message());
    }
  }

  writeln(" assignment ops:");
  {
    try {
      var a = lhs.value(owned Value);
      a += rhs;
      writeln("  +=: ", a, " ", a.type:string, " ", a.pyType());
    } catch e {
      writeln("  +=: Exception: ", e.message());
    }
    try {
      var b = lhs.value(owned Value);
      b -= rhs;
      writeln("  -=: ", b, " ", b.type:string, " ", b.pyType());
    } catch e {
      writeln("  -=: Exception: ", e.message());
    }
    try {
      var c = lhs.value(owned Value);
      c *= rhs;
      writeln("  *=: ", c, " ", c.type:string, " ", c.pyType());
    } catch e {
      writeln("  *=: Exception: ", e.message());
    }
    try {
      var d = lhs.value(owned Value);
      d /= rhs;
      writeln("  /=: ", d, " ", d.type:string, " ", d.pyType());
    } catch e {
      writeln("  /=: Exception: ", e.message());
    }
    try {
      var e = lhs.value(owned Value);
      e %= rhs;
      writeln("  %=: ", e, " ", e.type:string, " ", e.pyType());
    } catch e {
      writeln("  %=: Exception: ", e.message());
    }
    try {
      var f = lhs.value(owned Value);
      f **= rhs;
      writeln("  **=: ", f, " ", f.type:string, " ", f.pyType());
    } catch e {
      writeln("  **=: Exception: ", e.message());
    }
    try {
      var g = lhs.value(owned Value);
      g &= rhs;
      writeln("  &=: ", g, " ", g.type:string, " ", g.pyType());
    } catch e {
      writeln("  &=: Exception: ", e.message());
    }
    try {
      var h = lhs.value(owned Value);
      h |= rhs;
      writeln("  |=: ", h, " ", h.type:string, " ", h.pyType());
    } catch e {
      writeln("  |=: Exception: ", e.message());
    }
    try {
      var i = lhs.value(owned Value);
      i ^= rhs;
      writeln("  ^=: ", i, " ", i.type:string, " ", i.pyType());
    } catch e {
      writeln("  ^=: Exception: ", e.message());
    }
    try {
      var j = lhs.value(owned Value);
      j <<= rhs;
      writeln("  <<=: ", j, " ", j.type:string, " ", j.pyType());
    } catch e {
      writeln("  <<=: Exception: ", e.message());
    }
    try {
      var k = lhs.value(owned Value);
      k >>= rhs;
      writeln("  >>=: ", k, " ", k.type:string, " ", k.pyType());
    } catch e {
      writeln("  >>=: Exception: ", e.message());
    }
  }
  writeln("========================");
}
