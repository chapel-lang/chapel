class foo {
  var a : int;
  proc bar {
    if a < 100
      then return true;
      else return false;
  }
}

var f = new borrowed foo();

if f.bar
  then writeln("Bar");
  else writeln("Not Bar");
