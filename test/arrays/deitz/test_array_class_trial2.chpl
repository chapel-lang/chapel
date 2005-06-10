class array1d {
  type t;
  var x1 : t;
  var x2 : t;
  var x3 : t;
  var x4 : t;
  function this(i : integer) : t {
    var result : t;
    select i {
      when 1 do result = x1;
      when 2 do result = x2;
      when 3 do result = x3;
      when 4 do result = x4;
      otherwise writeln("Out of bounds read");
    }
    writeln("Read on ", i, " returns ", result);
    return result;
  }
  function =this(i : integer, val : t) : t {
    select i {
      when 1 do x1 = val;
      when 2 do x2 = val;
      when 3 do x3 = val;
      when 4 do x4 = val;
      otherwise writeln("Out of bounds write");
    }
    writeln("Write on ", i, " sets ", val);
    return val;
  }
}

var a : array1d = array1d(integer);

a(1) = 4;
a(2) = 3;
a(3) = 2;
a(4) = 1;
writeln(a(1), " ", a(2), " ", a(3), " ", a(4));
