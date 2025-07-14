//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
var myString = "|".join("a","10","d");
writeln(myString); // prints: "a|10|d"
/* STOP_EXAMPLE_0 */

assert(myString == "a|10|d");

/* START_EXAMPLE_1 */
var tup = ("a","10","d");
var myJoinedTuple = "|".join(tup);
writeln(myJoinedTuple); // prints: "a|10|d"

var myJoinedArray = "|".join(["a","10","d"]);
writeln(myJoinedArray); // prints: "a|10|d"
/* STOP_EXAMPLE_1 */

assert(myJoinedTuple == "a|10|d");
assert(myJoinedArray == "a|10|d");
