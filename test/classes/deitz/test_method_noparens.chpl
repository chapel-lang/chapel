class foo {
  var a : int;
  fun bar {
    if a < 100
      then return true;
      else return false;
  }
}

var f = foo();

if f.bar
  then writeln("Bar");
  else writeln("Not Bar");

