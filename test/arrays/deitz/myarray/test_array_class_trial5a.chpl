class array1d {
  type t;
  var x1 : t;
  var x2 : t;
  var x3 : t;
  proc this(i : int) ref {
    select i {
      when 1 do return x1;
      when 2 do return x2;
      when 3 do return x3;
      otherwise writeln("[Out of bounds read]");
    }
    return x1;
  }
}

var a : borrowed array1d(int) = new borrowed array1d(int);

a(1) = 3;
a(2) = 2;
a(3) = 1;
writeln(a(1), a(2), a(3));

class array2d {
  type t;
  var data : unmanaged array1d(t) = new unmanaged array1d(t);

  proc deinit() {
    delete data;
  }

  proc this(i : int, j : int) ref {
    return data((i - 1) * 2 + j);
  }
}

var a2 : borrowed array2d(int) = new borrowed array2d(int);

a2(1, 1) = 4;
writeln(a2(1, 1));
