class array1d {
  type t;
  var x1 : t;
  var x2 : t;
  var x3 : t;
  function this(i : integer) : t {
    var result : t;
    select i {
      when 1 do result = x1;
      when 2 do result = x2;
      when 3 do result = x3;
      otherwise writeln("[Out of bounds read]");
    }
    write("[Read on ", i, " returns ", result, "]");
    return result;
  }
  function =this(i : integer, val : t) : t {
    select i {
      when 1 do x1 = val;
      when 2 do x2 = val;
      when 3 do x3 = val;
      otherwise writeln("[Out of bounds write]");
    }
    writeln("[Write on ", i, " sets ", val, "]");
    return val;
  }
}

var a : array1d = array1d(integer);

a(1) = 3;
a(2) = 2;
a(3) = 1;
writeln(a(1), a(2), a(3));

class array2d {
  type t;
  var data : array1d = array1d(t);
  function this(i : integer, j : integer) : t {
    return data((i - 1) * 2 + j);
  }
  function =this(i : integer, j : integer, val : t) : t {
    data((i - 1) * 2 + j) = val;
    return val;
  }
}

var a2 : array2d = array2d(integer);

a2(1, 1) = 4;
writeln(a2(1, 1));
