config const verbose = false;
type types = (imag(32), imag(64));

inline proc vals(param idx) param {
  select idx {
    when 0 do return 0.0i;
    when 1 do return 1.0i;
    when 2 do return -2.0i;
    when 3 do return 4.0i;
    when 4 do return -8.0i;
    when 5 do return 0.5i;
    when 6 do return 0.125i;
    when 7 do return 16.0i;
    otherwise compilerError("Invalid index");
  }
}
param vals_size = 8;


for param i in 0..<types.size {
  type t = types(i);

  for param m in 0..<vals_size {
    for param n in 0..<vals_size {
      param a = vals(m):t;
      param b = vals(n):t;

      param lt = a < b;
      param gt = a > b;
      param le = a <= b;
      param ge = a >= b;
      param eq = a == b;
      param ne = a != b;
      param minVal = min(a, b);
      param maxVal = max(a, b);

      const aa = a;
      const bb = b;
      const lt2 = aa < bb;
      const gt2 = aa > bb;
      const le2 = aa <= bb;
      const ge2 = aa >= bb;
      const eq2 = aa == bb;
      const ne2 = aa != bb;
      const minVal2 = min(aa, bb);
      const maxVal2 = max(aa, bb);

      if verbose then
        writeln(a, " X ", b, ": ",
                "a < b = ", lt, ", ",
                "a > b = ", gt, ", ",
                "a <= b = ", le, ", ",
                "a >= b = ", ge, ", ",
                "a == b = ", eq, ", ",
                "a != b = ", ne, ", ",
                "min(a, b) = ", minVal, ", ",
                "max(a, b) = ", maxVal);

      // check for consistency between param and non-param versions of the comparisons
      if lt != lt2 then
        writeln("ERROR: ", a, " < ", b, " = ", lt, " != ", lt2);
      if gt != gt2 then
        writeln("ERROR: ", a, " > ", b, " = ", gt, " != ", gt2);
      if le != le2 then
        writeln("ERROR: ", a, " <= ", b, " = ", le, " != ", le2);
      if ge != ge2 then
        writeln("ERROR: ", a, " >= ", b, " = ", ge, " != ", ge2);
      if eq != eq2 then
        writeln("ERROR: ", a, " == ", b, " = ", eq, " != ", eq2);
      if ne != ne2 then
        writeln("ERROR: ", a, " != ", b, " = ", ne, " != ", ne2);
      if minVal != minVal2 then
        writeln("ERROR: min(", a, ", ", b, ") = ", minVal, " != ", minVal2);
      if maxVal != maxVal2 then
        writeln("ERROR: max(", a, ", ", b, ") = ", maxVal, " != ", maxVal2);

      // check that the values are correct by casting to real and comparing to the real values
      if lt != (a:real < b:real) then
        writeln("ERROR: ", a, " < ", b, " = ", lt, " != ", (a:real < b:real));
      if gt != (a:real > b:real) then
        writeln("ERROR: ", a, " > ", b, " = ", gt, " != ", (a:real > b:real));
      if le != (a:real <= b:real) then
        writeln("ERROR: ", a, " <= ", b, " = ", le, " != ", (a:real <= b:real));
      if ge != (a:real >= b:real) then
        writeln("ERROR: ", a, " >= ", b, " = ", ge, " != ", (a:real >= b:real));
      if eq != (a:real == b:real) then
        writeln("ERROR: ", a, " == ", b, " = ", eq, " != ", (a:real == b:real));
      if ne != (a:real != b:real) then
        writeln("ERROR: ", a, " != ", b, " = ", ne, " != ", (a:real != b:real));
      if minVal:real != min(a:real, b:real) then
        writeln("ERROR: min(", a, ", ", b, ") = ", minVal, " != ", min(a:real, b:real));
      if maxVal:real != max(a:real, b:real) then
        writeln("ERROR: max(", a, ", ", b, ") = ", maxVal, " != ", max(a:real, b:real));
    }
  }
}
