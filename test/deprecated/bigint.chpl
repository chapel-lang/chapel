/*
  Noakes 2016/09/12

  The class  GMP.BigInt is being deprecated in favor of
  the record BigInteger.Bigint
*/

use GMP;

proc main() {
  var   a = new BigInt(234958444);
  const b = new BigInt("4847382292989382987395534934347");
  var   c = new BigInt();

  c.mul(a, b);
  writeln(c);

  var   d = new BigInt();
  d.fac_ui(100 : uint);
  writeln(d);

  delete d;
  delete c;
  delete b;
  delete a;
}
