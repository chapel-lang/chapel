/* testing that funs specified with the 'inline' keyword will be inlined
 * testing that funs passed actual ref arguments/formal parameters
 * testing that funs with intents to ensure copied backs happen for out/inout
 */

class Foo {
  var y : int;
}

proc main() {
  var f : unmanaged Foo = new unmanaged Foo();

  /* setter method will be created, temporary should not be created for f
   * since formal param this will be a ref */
  f.y = 1;

  /* this method should be inlined because of the 'inline' keyword
   */
  leaf(f);

  var one   : int = 1;
  var two   : int = 2;
  var three : int = 3;
  var four  : int = 4;

  /* temporaries should be created for all four variables and
   * there should be two copy back assignments created for variable
   * two and three since the formal param at that position is out */
  bar(one, two, three, four);

  writeln(one);
  writeln(two);
  writeln(three);
  writeln(four);

  delete f;
}

inline proc leaf(x : Foo) : Foo {
  return x;
}

proc bar(in a: int, out b: int, inout c : int, const d : int) {
  a = 11;
  b = 22;
  c = 33;
}
