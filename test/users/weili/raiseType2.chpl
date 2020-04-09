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
writeln(h1.type:string);
writeln(h2.type:string);
