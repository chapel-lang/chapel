var a: integer = 1;                      -- module scope
var n: integer = 14;

function b(c: integer): integer {        -- function param scope
  var d: integer = 4;                    -- function scope
  writeln("c is: ", c);
  writeln("d is: ", d);

  return 2;
}

class e {
  var f: integer = 6;                    -- class scope
  function g(h: integer): integer {
    var i: integer = 9;
--    writeln("f is: ", f);
    writeln("h is: ", h);
    writeln("i is: ", i);
    return 7;
  }
}

function main() {
  writeln("a is: ", a);
  var myB: integer = b(3);
  writeln("b(3) is: ", myB);
  var myE: e = e();
  var myG: integer = myE.g(8);
  writeln("e.g(8) is: ", myG);

/*  
  for j in 1..n {                        -- for loop scope
    writeln("j is: ", j);
    var k: integer = 11;
    writeln("k is: ", k);
  }
*/
-- [l in 1..n] writeln("l is: ", l);     -- forall expr scope
  {                                      -- block stmt scope
    var m: integer = 13;
    {
      var o: integer = 15;

      writeln("m is: ", m);
      writeln("o is: ", o);
    }
  }

--  var q: integer = let p = a+n in p*p;   -- let expr scope
}
