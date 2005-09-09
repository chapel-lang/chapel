class array1d {
  type t;
  var x1 : t;
  var x2 : t;
  var x3 : t;
  function indexedby(i : integer) : t {
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
  function =indexedby(i : integer, val : t) : t {
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

var a : array1d(integer) = array1d(integer);

a.indexedby(1) = 3;
a.indexedby(2) = 2;
a.indexedby(3) = 1;
writeln(a.indexedby(1), a.indexedby(2), a.indexedby(3));

class array2d {
  type t;
  var data : array1d(t) = array1d(t);
  function indexedby(i : integer, j : integer) : t {
    return data.indexedby((i - 1) * 2 + j);
  }
  function =indexedby(i : integer, j : integer, val : t) : t {
    data.indexedby((i - 1) * 2 + j) = val;
    return val;
  }
}

var a2 : array2d(integer) = array2d(integer);

a2.indexedby(1, 1) = 4;
writeln(a2.indexedby(1, 1));
