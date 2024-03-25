
/*****
      ex_op.chpl -
      Examples of operators.  See text for details.

      c 2015-2018 Primordial Machine Vision Systems
*****/

writeln("-(1.0 - 2.0i)   = ", -(1.0 - 2.0i));

writeln("1.0i * -2.0i    = ", 1.0i * -2.0i);
writeln("1.0i * 2.0      = ", 1.0i * 2.0);
writeln("3.0i / 2.0i     = ", 3.0i / 2.0i);
writeln("3.0i / 2.0      = ", 3.0i / 2.0);

writeln("\"concat\" + true = ", "concat" + true:string);

writeln("9 % 5     = ", 9 % 5);
writeln("(-9) % 5  = ", (-9) % 5);
writeln("9 % (-5)  = ", 9 % (-5));

/* ineg == -86, ipos == 85, u == 204 */
const ineg : int(8) = 0b10101010 : int(8);
const ipos : int(8) = 0b01010101 : int(8);
const u : uint(8) = 0b11001100 : uint(8);
writef("ineg | u     = %08bu (%u)\n", ineg | u, ineg | u);
writef("ipos | u     = %08bu (%u)\n", ipos | u, ipos | u);
writef("ineg | ipos  = %08bu (%u)\n", ineg | ipos, ineg | ipos);
writef("ineg & u     = %08bu (%u)\n", ineg & u, ineg & u);
writef("ipos & u     = %08bu (%u)\n", ipos & u, ipos & u);
writef("ineg & ipos  = %08bu (%u)\n", ineg & ipos, ineg & ipos);
writef("ineg ^ u     = %08bu (%u)\n", ineg ^ u, ineg ^ u);
writef("ipos ^ u     = %08bu (%u)\n", ipos ^ u, ipos ^ u);
writef("ineg ^ ipos  = %08bu (%u)\n", ineg ^ ipos, ineg ^ ipos);
writef("~ineg        = %08bu (%u)\n", ~ineg, ~ineg);
writef("~ipos        = %08bu (%u)\n", ~ipos, ~ipos);
writef("~u           = %08bu (%u)\n", ~u, ~u);

writef("ineg << 2    = %08bu (%u)\n", ineg << 2, ineg);
writef("ipos << 2    = %08bu (%u)\n", ipos << 2, ipos);
writef("u << 2       = %08bu (%u)\n", u << 2, u);
writef("ineg >> 2    = %08bu (%u)\n", ineg >> 2, ineg);
writef("ipos >> 2    = %08bu (%u)\n", ipos >> 2, ipos);
writef("u >> 2       = %08bu (%u)\n", u >> 2, u);

proc tstfn(i : int) {
  writeln("called tstfn with ", i);
}
tstfn[2];

const spos : int(16) = 0xcafe : int(16);
writef("0x%xu : int(8)      = 0x%xu\n", spos, spos : int(8));

/* 1234 is 0x4D2.  Chapel can't read hex notation for integers when casting
   from a string. */
const icast = "1234" : int;
writef("\"1234\" : int         = 0x%xi\n", icast);
const ucast = "1234" : uint(8);
writef("\"1234\" : uint(8)     = 0x%xu\n", ucast);
const imcast = "6.626e-34i" : imag;
writef("\"6.626e-34i\" : imag  = %m\n", imcast);
const bcast = "false" : bool;
writef("\"false\" : bool       = %s\n", bcast);
