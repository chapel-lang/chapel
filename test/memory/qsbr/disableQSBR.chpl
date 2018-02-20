use Memory;

extern proc chpl_qsbr_checkpoint();
extern proc chpl_qsbr_defer_deletion(c_void_ptr);
extern proc chpl_qsbr_enable();
extern proc chpl_qsbr_disable();

// TODO: Find a way to test the validity of this... I.E: Check deferList

// Defer deletion (without checkpoint)
for 1 .. 1024 * 1024 {
	chpl_qsbr_defer_deletion((new object()) : c_void_ptr);
}

printMemAllocStats();

// Disable...
chpl_qsbr_disable();

// Insert another object but keep reference to it
class Obj { var x : int; var y : int; var z : int;}
var obj = new Obj(1,2,3);
chpl_qsbr_defer_deletion(obj : c_void_ptr);

// The above objects inserted before being disabled should be reclaimed...
chpl_qsbr_checkpoint();

printMemAllocStats();

// After enabling, the remaining `obj` should be reclaimed
chpl_qsbr_enable();
chpl_qsbr_checkpoint();

printMemAllocStats();
