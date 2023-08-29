// Passing a sync variable to a non-sync 'ref' formal, same underlying type.

/* See also:
test/classes/delete-free/shared/shared-default-arg-nil-ref.chpl
test/types/sync/ferguson/sync-coerce-to-ref.chpl
*/

proc fun1(ref x: int,    y) { compilerWarning("fun1 int");    }
proc fun1(ref x: sync,   y) { compilerWarning("fun1 sync");   }

proc fun2(ref x: int,    y: int) { compilerWarning("fun2 int");    }
proc fun2(ref x: sync,   y)      { compilerWarning("fun2 sync");   }

var q: int;
var sy: sync int;

fun1(sy, q);
fun2(sy, q);

compilerError("done");
