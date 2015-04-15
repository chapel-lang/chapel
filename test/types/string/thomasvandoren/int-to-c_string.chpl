const n = 5,
  cs = n: c_string,
  csc = n: c_string_copy,
  s = n: string;
writeln(n, " : ", typeToString(n.type));
writeln(cs, " : ", typeToString(cs.type));
writeln(csc, " : ", typeToString(csc.type));
writeln(s, " : ", typeToString(s.type));
