/* This test makes sure enum_mintype(enum type)
   works correctly. This information is used
   in binary I/O of enum values.
   */

proc myTypeToString(type t)
{
  if t == uint(8) then return "uint(8)";
  else if t == int(8) then return "int(8)";
  else if t == uint(16) then return "uint(16)";
  else if t == int(16) then return "int(16)";
  else if t == uint(32) then return "uint(32)";
  else if t == int(32) then return "int(32)";
  else if t == uint(64) then return "uint(64)";
  else if t == int(64) then return "int(64)";
}


{
  enum A {
    a = 1
  };

  writeln("A ", myTypeToString(chpl_enum_mintype(A)));

  enum B {
    a = -1
  };

  writeln("B ", myTypeToString(chpl_enum_mintype(B)));

  enum C {
    a = 0xff
  };

  writeln("C ", myTypeToString(chpl_enum_mintype(C)));

  enum D {
    a = -0x80
  };

  writeln("D ", myTypeToString(chpl_enum_mintype(D)));

  enum E {
    a = 0xff,
    b
  };

  writeln("E ", myTypeToString(chpl_enum_mintype(E)));


  enum F {
    a = -0x81
  };

  writeln("F ", myTypeToString(chpl_enum_mintype(F)));

  enum G {
    a = 0xff,
    b = -0x80
  };

  writeln("G ", myTypeToString(chpl_enum_mintype(G)));
}
{
  enum A {
    a = 0x100
  };

  writeln("A ", myTypeToString(chpl_enum_mintype(A)));

  enum B {
    a = -0x100
  };

  writeln("B ", myTypeToString(chpl_enum_mintype(B)));

  enum C {
    a = 0xffff
  };

  writeln("C ", myTypeToString(chpl_enum_mintype(C)));

  enum D {
    a = -0x8000
  };

  writeln("D ", myTypeToString(chpl_enum_mintype(D)));


  enum E {
    a = 0xffff,
    b
  };

  writeln("E ", myTypeToString(chpl_enum_mintype(E)));

  enum F {
    a = -0x8001
  };

  writeln("F ", myTypeToString(chpl_enum_mintype(F)));

  enum G {
    a = 0xffff,
    b = -0x8000
  };

  writeln("G ", myTypeToString(chpl_enum_mintype(G)));
}
{
  enum A {
    a = 0x10000
  };

  writeln("A ", myTypeToString(chpl_enum_mintype(A)));

  enum B {
    a = -0x10000
  };

  writeln("B ", myTypeToString(chpl_enum_mintype(B)));

  enum C {
    a = 0xffffffff
  };

  writeln("C ", myTypeToString(chpl_enum_mintype(C)));

  enum D {
    a = -0x80000000
  };

  writeln("D ", myTypeToString(chpl_enum_mintype(D)));

  enum E {
    a = 0xffffffff,
    b
  };

  writeln("E ", myTypeToString(chpl_enum_mintype(E)));


  enum F {
    a = -0x80000001
  };

  writeln("F ", myTypeToString(chpl_enum_mintype(F)));

  enum G {
    a = 0xffffffff,
    b = -0x80000000
  };

  writeln("G ", myTypeToString(chpl_enum_mintype(G)));
}
{
  enum A {
    a = 0x100000000
  };

  writeln("A ", myTypeToString(chpl_enum_mintype(A)));

  enum B {
    a = -0x100000000
  };

  writeln("B ", myTypeToString(chpl_enum_mintype(B)));

  enum C {
    a = 0xffffffffffffffff
  };

  writeln("C ", myTypeToString(chpl_enum_mintype(C)));

  enum E {
    a = -0x7fffffffffffffff
  };

  writeln("E ", myTypeToString(chpl_enum_mintype(E)));
}
