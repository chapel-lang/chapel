//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use Regex;

var myRegex = new regex("some");

inline proc doSomethingIfMatched() do writeln('YES');
inline proc doSomethingIfNotMatched() do writeln('NO');
inline proc match() do return myRegex.match("somebody once told me");

{
/* START_EXAMPLE_0 */
var m: regexMatch = match();
if m then doSomethingIfMatched();
if !m then doSomethingIfNotMatched();
/* STOP_EXAMPLE_0 */
}

/* START_EXAMPLE_1 */
if myRegex.match("some string") {
  doSomethingIfMatched();
}
/* STOP_EXAMPLE_1 */
