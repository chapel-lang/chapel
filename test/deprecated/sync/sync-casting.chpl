// sync casts added to avoid errors for = between types
// these are deprecated because the = between types for sync is too

proc test16() {
  writeln("test16");
  var s1: sync bool;
  var s2:sync int = s1:sync int;
}
test16();

/* doesn't compile but we have cast fns to support it
proc test17() {
  writeln("test17");
  type t = sync int;
  var one = 1;
  var s1:sync int = one:t;
}
test17();
*/

proc test18() {
  writeln("test18");
  var s1: single bool;
  var s2:single int = s1:single int;
}
test18();

/* doesn't compile but we have cast fns to support it
proc test19() {
  writeln("test19");
  type t = single int;
  var one = 1; 
  var s1:single int = one:t;
}
test19();
*/
