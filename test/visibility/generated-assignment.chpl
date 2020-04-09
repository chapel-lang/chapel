/* Before PR ##10512
   this test failed to compile with the error:

    $CHPL_HOME/modules/internal/ChapelArray.chpl:3893: error: type mismatch in assignment from I to I
    $CHPL_HOME/modules/internal/ChapelArray.chpl:3893: note: unresolved call had id 1436887
 */

use CyclicDist, Map;

proc main() {
  var m = new map(string, I);;
  m['a'] = new I();

  const space = {1..m.size};
  const dom: domain(1) dmapped Cyclic(startIdx=1) = space;

  var arr: [dom] I = for v in m.values() do v;
}

record I {

  proc init=(const ref i: I) { }

  proc init() { }

}

proc =(ref lhs: I, const ref rhs: I) { }
