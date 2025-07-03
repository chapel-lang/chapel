//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
var myBytes = b"|".join(b"a",b"10",b"d");
writeln(myBytes); // prints: "a|10|d"
/* STOP_EXAMPLE_0 */

assert(myBytes == b"a|10|d");

/* START_EXAMPLE_1 */
var tup = (b"a",b"10",b"d");
var myJoinedTuple = b"|".join(tup);
writeln(myJoinedTuple); // prints: "a|10|d"

var myJoinedArray = b"|".join([b"a",b"10",b"d"]);
writeln(myJoinedArray); // prints: "a|10|d"
/* STOP_EXAMPLE_1 */

assert(myJoinedTuple == b"a|10|d");
assert(myJoinedArray == b"a|10|d");
