class array1d {
  type t;
  var x1 : t;
  var x2 : t;
  var x3 : t;
  proc this(i : int) ref : t {
    writeln("Write on ", i);
    select i {
      when 1 do return x1;
      when 2 do return x2;
      when 3 do return x3;
      otherwise writeln("Out of bounds write");
    }
    return x1;
  }
  proc this(i : int) : t {
    writeln("Read on ", i);
    select i {
      when 1 do return x1;
      when 2 do return x2;
      when 3 do return x3;
      otherwise writeln("Out of bounds read");
    }
    return x1;
  }

}

var a : borrowed array1d(int) = new borrowed array1d(int);

a(1) = 2;
writeln(a(1));
