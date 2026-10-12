config const verbose = false;
config param width = 32;
type types = (real(width), imag(width), complex(width*2));

inline proc vals(param idx) param {
  select idx {
    when 0 do return 0.0;
    when 1 do return 1.0;
    when 2 do return -2.0;
    when 3 do return 4.0;
    when 4 do return -8.0;
    when 5 do return 0.5;
    when 6 do return 0.125;
    when 7 do return 16.0;
    otherwise compilerError("Invalid index");
  }
}
param vals_size = 8;
inline proc nonzero(param idx) param {
  if idx == 0 then return 1.0;
  else return vals(idx);
}

inline proc cvals(param idx) param {
  select idx {
    when 0 do return 0.0 + 0.0i;
    when 1 do return 1.0 + 1.0i;
    when 2 do return -1.0 + 2.0i;
    when 3 do return 2.0 - 0.5i;
    when 4 do return -0.5 + 4.0i;
    when 5 do return 0.25 - 0.125i;
    when 6 do return -8.0 - 16.0i;
    when 7 do return 4.0 + 0.5i;
    otherwise compilerError("Invalid index");
  }
}
param cvals_size = 8;
inline proc cnonzero(param idx) param {
  if idx == 0 then return 1.0 + 1.0i;
  else return cvals(idx);
}

for param i in 0..<types.size {
  for param j in 0..<types.size {
    type aT = types(i);
    type bT = types(j);

    for param m in 0..<vals_size {
      for param n in 0..<vals_size {
        param a = vals(m):aT;
        param b = vals(n):bT;
        param bnonzero = nonzero(n):bT;
        {
          // mul(a, b);
          param c1 = a * b;
          var aa = a, bb = b, c2 = aa * bb;
          if verbose then
            writeln(a.type:string, " X ", b.type:string, ": ",
                    a, " * ", b, " = ", c1, " == ", c2);
          if c1 != c2 then
            writeln("FAILED: a: ", a.type:string, " b: ", b.type:string,
                    " c1: ", c1.type:string, " c2: ", c2.type:string);
        }

        {
          // div(a, bnonzero);
          param c1 = a / bnonzero;
          var aa = a, bb = bnonzero, c2 = aa / bb;
          if verbose then
            writeln(a.type:string, " / ", bnonzero.type:string, ": ",
                    a, " / ", bnonzero, " = ", c1, " == ", c2);
          if c1 != c2 then
            writeln("FAILED: a: ", a.type:string, " b: ", bnonzero.type:string,
                    " c1: ", c1.type:string, " c2: ", c2.type:string);
        }

        if m < cvals_size && n < cvals_size {
          param a = if isComplexType(aT) then cvals(m):aT
                    else if isImagType(aT) then cvals(m).im:aT
                    else cvals(m).re:aT;
          param b = if isComplexType(bT) then cvals(n):bT
                    else if isImagType(bT) then cvals(n).im:bT
                    else cvals(n).re:bT;
          param bnonzero = if isComplexType(bT) then cnonzero(m):bT
                          else if isImagType(bT) then cnonzero(m).re:bT
                          else cnonzero(m).re:bT;

          {
            // mul(a, b);
            param c1 = a * b;
            var aa = a, bb = b, c2 = aa * bb;
            if verbose then
              writeln(a.type:string, " * ", b.type:string, ": ",
                      a, " * ", b, " = ", c1, " == ", c2);
            if c1 != c2 then
              writeln("FAILED: a: ", a.type:string, " b: ", b.type:string,
                      " c1: ", c1.type:string, " c2: ", c2.type:string);
          }

          {
            // div(a, bnonzero);
            param c1 = a / bnonzero;
            var aa = a, bb = bnonzero, c2 = aa / bb;
            if verbose then
              writeln(a.type:string, " / ", bnonzero.type:string, ": ",
                      a, " / ", bnonzero, " = ", c1, " == ", c2);
            if c1 != c2 then
              writeln("FAILED: a: ", a.type:string, " b: ", bnonzero.type:string,
                      " c1: ", c1.type:string, " c2: ", c2.type:string);
          }
        }
      }
    }

  }
}
