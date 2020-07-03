/*
 * This test at one point hit a leak in reinitString() when run on
 * more than one locale. #7549
 */
var s = "1" * 8;

on Locales[numLocales-1] {

  var t = "1" * 12;
  var tx = t.localize(); // gets tx.owned == false

  writeln(tx);

  // ENGIN: I am keeping the comment below for record-keeping purposes. But it
  // is not true anymore; we use the factory functions in the path
  // described.
  // Since s is on locale 0 and here is numLocales-1, proc =()'s
  // helpMe() goes down the "remote copy" path calling reinitString()
  // with needToCopy=false.
  tx = s;

  writeln(tx);
}
