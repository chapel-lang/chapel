
config const flag = true;

proc main() {
  var r : range = 1..10;
  // Same element type, same shape, but compiler considers loop-exprs to be
  // different types
  var x = if flag then for i in r do 5
          else for i in r do -5;
  
  writeln(x);
}
