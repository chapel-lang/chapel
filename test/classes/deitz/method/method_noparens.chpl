class foo {
  var a : int;
  proc bar {
    if a < 100
      then return true;
      else return false;
  }
}

var ownF = new owned foo();
var f = ownF.borrow();

if f.bar
  then writeln("Bar");
  else writeln("Not Bar");
