record mytuple {
  type t1;
  type t2;
  var f1 : t1;
  var f2 : t2;
  function this(parameter i : integer) var where i == 1 {
    return f1;
  }
  function this(parameter i : integer) var where i == 2 {
    return f2;
  }
}

function write(val : mytuple) {
  write("(", val.f1, ", ", val.f2, ")");
}

var t : mytuple(integer, float);

t(1) = 12;
t(2) = 14.0;

writeln(t);
