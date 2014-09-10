class array1d {
  type t;
  var x1 : t;
  var x2 : t;
  var x3 : t;
  proc this(i : int) ref : t {
    select i {
      when 1 do return x1;
      when 2 do return x2;
      when 3 do return x3;
      otherwise writeln("[Out of bounds read]");
    }
    return x1;
  }
}

var a : array1d(int) = new array1d(int);

a(1) = 3;
a(2) = 2;
a(3) = 1;
writeln(a(1), a(2), a(3));
