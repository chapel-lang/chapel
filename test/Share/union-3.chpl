union Weigth {
  var i : int;
  var s : string;
  proc is_string? {
    typeselect(this) {
      when s return true;
      otherwise return false;
    }
  }
}

var x : Weight = 5:Weight.i;
if (x.is_string?)
  writeln("string ", x.s);
else
  writeln("int ", x.i);
