use BigInteger;
import IO.string;

config const executeLocale = 0;

var a: bigint = 0xBEEF;
var res:bigint;

on Locales[min(Locales.domain.high, executeLocale)] {
  shiftLeft(res, a, 8);
  writeln("0x%Xu".format(res:int));
  shiftRight(res, a, -8);
  writeln("0x%Xu".format(res:int));

  shiftRight(res, a, 8);
  writeln("0x%Xu".format(res:int));
  shiftLeft(res, a, -8);
  writeln("0x%Xu".format(res:int));
}
