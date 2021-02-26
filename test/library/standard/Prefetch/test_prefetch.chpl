use Prefetch;
use CPtr;

var x = 8;
var y = c_ptrTo(x);
prefetch(y);
writeln(x);
