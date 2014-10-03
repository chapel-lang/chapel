enum color {red, blue};

record R {
  var i: int;
}

const b: bool = true;
const n: int = 1;
const u: uint = 2;
const r: real = 3.4;
const i: imag = 5.6i;
const c: complex = 7.8 + 9.0i;
const e: color = color.red;
const s: string = "hi";
const myR: R = new R(i=11);

on Locales[(here.id + 1) % numLocales] {
  writeln("b is ", b, " and is on locale: ", b.locale.id);
  writeln("n is ", n, " and is on locale: ", n.locale.id);
  writeln("u is ", u, " and is on locale: ", u.locale.id);
  writeln("r is ", r, " and is on locale: ", r.locale.id);
  writeln("i is ", i, " and is on locale: ", i.locale.id);
  writeln("c is ", c, " and is on locale: ", c.locale.id);
  writeln("e is ", e, " and is on locale: ", e.locale.id);
  // This line should be commented out once stringConstRepl.future is retired
  // writeln("s is ", s, " and is on locale: ", s.locale.id);
  writeln("myR is ", myR, " and is on locale: ", myR.locale.id);
}
