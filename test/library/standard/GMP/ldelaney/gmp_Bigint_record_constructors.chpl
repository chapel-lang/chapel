use BigInteger, IO;

proc main(){
  var x1          = new bigint();
  var x3          = new bigint(100);
  var x4          = new bigint("1100101", 2);
  var x5          = new bigint(x4);
  var x6 : bigint = x5;
  var x7          = x3;

  x1.writeThis(stdout);
  writeln();

  x3.writeThis(stdout);
  writeln();

  x4.writeThis(stdout);
  writeln();

  x5.writeThis(stdout);
  writeln();

  x6.writeThis(stdout);
  writeln();

  x7.writeThis(stdout);
  writeln();
}
