union Weigth {
  var i : integer;
  var s : string;
  function is_string? {
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
  writeln("integer ", x.i);
