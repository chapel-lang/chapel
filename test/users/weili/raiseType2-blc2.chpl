proc raiseType(x) {
      if (x.type==bool) {
              var x:int;
              return x;
      }
      if (x.type==int) {
              var x:real;
              return x;
      }
}

var h1 = raiseType(true);
var h2 = raiseType(3);
writeln(typeToString(h1.type));
writeln(typeToString(h2.type));
