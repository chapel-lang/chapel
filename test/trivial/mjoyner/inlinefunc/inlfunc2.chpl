/* testing that functions specified with the pragma inline will be inlined
 * testing that functions passed actual ref arguments/formal parameters
 * testing that functions with intents to ensure copied backs happen for out/inout
 */

class Foo {
  var y : integer;
}

function main() {
  var f : Foo = Foo();
  /* setter method will be created, temporary should not be created for f
   * since formal param this will be a ref */
  f.y = 1;
 
  /* this method should be inlined because of the pragma "inline"
   */
  leaf(f);

  var one: integer = 1;
  var two: integer = 2;
  var three: integer = 3;
  var four : integer = 4;
  
  /* temporaries should be created for all four variables and 
   * there should be two copy back assignments created for variable 
   * two and three since the formal param at that position is out */ 
  bar(one, two, three, four);
  writeln(one);
  writeln(two);
  writeln(three);
  writeln(four);
 
}

pragma "inline" function leaf(x : Foo) : Foo {
  return x;
}

function bar(in a: integer, out b: integer, inout c : integer, const d : integer) {    
  a = 11;
  b = 22;
  c = 33;
}
