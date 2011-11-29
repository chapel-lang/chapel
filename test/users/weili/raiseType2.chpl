proc raiseType(type x) {
      if (x==bool) {
              var x:int;
              return x;
      }
      if (x==int) {
              var x:real;
              return x;
      }
}

var h1 = raiseType(bool);
var h2 = raiseType(int);
writeln(typeToString(h1.type));
writeln(typeToString(h2.type));
