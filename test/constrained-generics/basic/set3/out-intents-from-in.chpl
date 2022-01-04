// Testing out-intent formals, initialized from in-intent formals.
// As of this writing, this requires both `=` and `chpl__initCopy`.

interface IFC(Self) {
  type AT;
  proc chpl__initCopy(rhs: Self, definedConst: bool): Self;
  proc chpl__initCopy(rhs: AT,   definedConst: bool): AT;
  operator =(ref lhs: Self, rhs: Self);
  operator =(ref lhs: AT,   rhs: AT);
}

// Naming conventions
// * "ic" = interface-constrained function
// * the first capital letter indicates the argument intent for the formal
//   `x` of the `Self` type
// * the second capital letter indicates the argument intent for the formal
//   `y` of the `AT` type
// * for an out intent ("O" versions) there are two formals:
//    - out intent `x` and/or `y`
//    - in intent `z` -- it initializes the out-intent formal

proc icDO(x1: ?Q1, out y1: x1.AT, in z1: x1.AT)
  where Q1 implements IFC
{
  writeln("  icDO");
  y1 = z1;
}

proc icRO(ref x2: ?Q2, out y2: x2.AT, in z2: x2.AT)
  where Q2 implements IFC
{
  writeln("  icRO");
  y2 = z2;
}

proc icOD(in z3: ?Q3, out x3: Q3, y3: z3.AT)
  where Q3 implements IFC
{
  writeln("  icOD");
  x3 = z3;
}

proc icOR(in z4: ?Q4, out x4: Q4, ref y4: z4.AT)
  where Q4 implements IFC
{
  writeln("  icOR");
  x4 = z4;
}

proc icOO(in z5s: ?Q5, in z5a: Q5.AT, out x5: Q5, out y5: Q5.AT)
  where Q5 implements IFC
{
  writeln("  icOO");
  x5 = z5s;
  y5 = z5a;
}

proc wrapDO(x1: ?Q1, out y1: x1.AT, in z1: x1.AT)
  where Q1 implements IFC
{
  writeln("wrapDO");
  icDO(x1, y1, z1);
  // no icRO
  // no icOD
  // no icOR
  // no icOO
}

proc wrapRO(ref x2: ?Q2, out y2: x2.AT, in z2: x2.AT)
  where Q2 implements IFC
{
  writeln("wrapRO");
  icDO(x2, y2, z2);
  icRO(x2, y2, z2);
  icOD(x2, x2, y2);
  icOR(x2, x2, z2);
  icOO(x2, z2, x2, y2);
}

proc wrapOD(in z3: ?Q3, out x3: Q3, y3: z3.AT)
  where Q3 implements IFC
{
  writeln("wrapOD");
  // no icDO
  // no icRO
  icOD(z3, x3, y3);
  // no icOR
  // no icOO
}

proc wrapOR(in z4: ?Q4, out x4: Q4, ref y4: z4.AT)
  where Q4 implements IFC
{
  writeln("wrapOR");
  icDO(z4, y4, y4);
  icRO(z4, y4, y4);
  icOD(z4, x4, y4);
  icOR(z4, x4, y4);
  icOO(z4, y4, x4, y4);
}

proc wrapOO(in z5s: ?Q5, in z5a: Q5.AT, out x5: Q5, out y5: Q5.AT)
  where Q5 implements IFC
{
  writeln("wrapOO");
  icDO(z5s, y5, z5a);
  icRO(z5s, y5, z5a);
  icOD(z5s, x5, z5a);
  icOR(z5s, x5, z5a);
  icOO(z5s, z5a, x5, y5);
}

int implements IFC;
proc int.AT type return string;

{ var str1: string;
  wrapDO(111, str1, "  wrapDO done");
  writeln(str1);
}
{ var int2: int;
  var str2: string;
  wrapRO(int2, str2, "  wrapRO done");
  writeln(str2);
}
{ var int3: int;
  wrapOD(333, int3, "  wrapOD finished");
  writeln("  wrapOD done ", int3);
}
{ var int4: int;
  var str4 = "  wrapOR done ";
  wrapOR(444, int4, str4);
  writeln(str4, int4);
}
{ var int5: int;
  var str5: string;
  wrapOO(555, "  wrapOO done ", int5, str5);
  writeln(str5, int5);
}
