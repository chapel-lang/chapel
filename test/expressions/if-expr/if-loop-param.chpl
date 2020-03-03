
config param flag = true;

proc main() {
  var r : range = 1..10;
  var x = if flag then for i in r do 5
          else for i in r do -5;
  
  writeln(x);
}
