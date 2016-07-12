use GMP;

var x = new BigInt(1);
on Locales[1 % numLocales] {
  var y: BigInt = 1;
  var k = y;
  on Locales[2 % numLocales] {
    var z = x + y;  // test with one variable local
    k = z;
    writeln(k);
    writeln(z);
    x = y + z;
    writeln(x);
    z.add(x, y);
    writeln(z);
    x.add(y, z);
    writeln(x);
    on Locales[3 % numLocales] {  // test with no variables local
      var z = x + y;
      k = z;
      writeln(k);
      writeln(z);
      x = y + z;
      writeln(x);
      z.add(x, y);
      writeln(z);
      x.add(y, z);
      writeln(x);
    }
  }
}
