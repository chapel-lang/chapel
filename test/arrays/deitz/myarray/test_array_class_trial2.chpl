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
      otherwise writeln("Out of bounds read");
    }
    return x1;
  }
}

var a1Own = new owned array1d(int);
var a1 : borrowed array1d(int) = a1Own.borrow();

a1(1) = 3;
a1(2) = 2;
a1(3) = 1;

var e1 = a1(1);
var e2 = a1(2);
var e3 = a1(3);

writeln(e1, " ", e2, " ", e3);



class array2d {
  type t;

  var data : unmanaged array1d(t) = new unmanaged array1d(t);

  proc this(i : int, j : int) ref {
    return data((i - 1) * 2 + j);
  }

  proc deinit() {
    delete data;
  }
}

var a2Own = new owned array2d(int);
var a2 : borrowed array2d(int) = a2Own.borrow();

a2(1, 1) = 4;
writeln(a2(1, 1));
