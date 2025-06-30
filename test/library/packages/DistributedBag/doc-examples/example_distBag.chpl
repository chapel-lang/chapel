//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use DistributedBag;

const targetedLocales = Locales;

/* START_EXAMPLE_1 */
var bag = new distBag(int, targetLocales=targetedLocales);
/* STOP_EXAMPLE_1 */

const taskId = here.maxTaskPar - 1;

/* START_EXAMPLE_2 */
bag.add(0, taskId);
bag.addBulk(1..100, taskId);
var (hasElt, elt) = bag.remove(taskId);
/* STOP_EXAMPLE_2 */

assert(bag.getSize() == 100);
writeln("hasElt = ", hasElt, " and elt = ", elt);

/* START_EXAMPLE_3 */
coforall loc in Locales do on loc {
  coforall taskId in 0..<here.maxTaskPar {
    var (hasElt, elt) = bag.remove(taskId);
    if hasElt {
      elt += 1;
      bag.add(elt, taskId);
    }
  }
}
/* STOP_EXAMPLE_3 */

proc do_something(x) {}

elt = numLocales * here.maxTaskPar - 1;

/* START_EXAMPLE_4 */
forall elt in bag do
  do_something(elt);

const size = bag.getSize();
const foundElt = bag.contains(elt);
bag.clear();
/* STOP_EXAMPLE_4 */

writeln("size = ", size);
writeln("foundElt = ", foundElt);
assert(bag.getSize() == 0);
