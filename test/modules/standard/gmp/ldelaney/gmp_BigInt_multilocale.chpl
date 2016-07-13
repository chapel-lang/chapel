use GMP;
var v1 = new BigInt(5);
on Locales[1 % numLocales] {
   var v2 = v1;
 //  writeln(v2);
   writeln("Hello from locale ", here.id);
   on Locales[2 % numLocales] {
      var v3 = v2;
      writeln("Hello from locale ", here.id);
      on Locales[3 % numLocales]{
      	 var v4 = v3 + v2;
	 var v5 = v4 ** 2;
	 v1 = v5;
      	 writeln("Hello from locale ", here.id);
	 if (v4 == 10) then writeln("YAY!");
	 writeln(v1);
      }
   }
}
writeln(v1);

/*var x = new BigInt(1);
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
*/