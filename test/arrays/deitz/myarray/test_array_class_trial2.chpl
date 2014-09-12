class array1d {
  type t;
  var x1 : t;
  var x2 : t;
  var x3 : t;
  var x4 : t;
  proc this(i : int) ref {
    select i {
      when 1 do return x1;
      when 2 do return x2;
      when 3 do return x3;
      when 4 do return x4;
      otherwise writeln("Out of bounds read");
    }
    return x1;
  }
}

var a : array1d(int) = new array1d(int);

a(1) = 4;
a(2) = 3;
a(3) = 2;
a(4) = 1;
var a1 = a(1);
var a2 = a(2);
var a3 = a(3);
var a4 = a(4);
writeln(a1, " ", a2, " ", a3, " ", a4);
