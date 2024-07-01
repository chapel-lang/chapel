// Underscoresa are disallowed in all named declarations except as components
// of tuple decls and formals.
var _ = 42;
var (x, y) = (_, _);
record _ {}
class _ {}
proc _(arg: int(?_)) {}
interface _ {}
enum _ {
    _
}

// loops get a special nicer error message
for _ in 1..10 {}
foreach _ in 1..10 {}
forall _ in 1..10 with (var _ = 42, + reduce _) {}
coforall _ in 1..10 {}

// can't capture into _ in a when
select (8, 9) {
 when (8, _) { writeln("Got 8"); }
}

// Imports can't rename to '_'
module A { var dummy: int; }
import this.A as _;
import this.A.{dummy as _};

// Use statements can't rename limitations to '_'
use A only dummy as _;

// Underscores ARE allowed in tuple declarations...
var (_, _) = (42, 42);
// .. tuple assignments
(_, _) = (42, 42);
// .. function formals (as far as our check is concerned; another check rules these out)
proc f(_: int) {}
// .. renames for use targets
use A as _;
