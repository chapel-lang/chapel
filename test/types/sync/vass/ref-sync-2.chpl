// Passing a sync variable to a non-sync 'ref' formal, same underlying type.

/* See also:
test/classes/delete-free/shared/shared-default-arg-nil-ref.chpl
test/types/sync/ferguson/sync-coerce-to-ref.chpl
*/

proc fun1(ref x: int,    y) { compilerWarning("fun1 int");    }
proc fun1(ref x: sync,   y) { compilerWarning("fun1 sync");   }
proc fun1(ref x: single, y) { compilerWarning("fun1 single"); }

proc fun2(ref x: int,    y: int) { compilerWarning("fun2 int");    }
proc fun2(ref x: sync,   y)      { compilerWarning("fun2 sync");   }

proc fun3(ref x: int,    y: int) { compilerWarning("fun3 int");    }
proc fun3(ref x: single, y)      { compilerWarning("fun3 single"); }

var q: int;
var sy: sync int;
var si: single int;

fun1(sy, q);
fun1(si, q);
fun2(sy, q);
fun3(si, q);

compilerError("done");
