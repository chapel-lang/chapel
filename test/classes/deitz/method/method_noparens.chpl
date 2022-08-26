class foo {
  var a : int;
  proc bar {
    if a < 100
      then return true;
      else return false;
  }
}

var f = (new owned foo()).borrow();

if f.bar
  then writeln("Bar");
  else writeln("Not Bar");
