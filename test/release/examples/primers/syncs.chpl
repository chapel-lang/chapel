// Sync

//
// This primer illustrates Chapel's sync variables.
//

// ``sync`` is a  type qualifier that can be applied to the Chapel primitive
// types ``nothing``, ``bool``, ``int``, ``uint``, ``real``, ``imag``,
// ``complex``, ``range``, ``bytes``, and ``string``; to enumerated types; and
// to class types and record types.
//

// Sync variables have an associated state that is either ``full`` or
// ``empty``.  If a sync variable is declared with an initializing expression,
// its state is set to ``full`` and its value is that of the expression.
// Without an initializing expression, a sync variable's state is set to
// ``empty`` and its value is the default value for the base type.
//
config const n = 7;

var si: sync int=1;  // state = full, value = 1
var sb: sync bool; // state = empty, value = false

// Because of their full/empty state, reads and writes to sync variables must
// be done with methods to make it clear how the full/empty state is treated in
// the operation.
// If a sync variable is not in the correct state for a given operation, the
// current task blocks until it is.

//
// Here are the methods defined on sync variables:
//

// The ``writeEF()`` method blocks until the state is ``empty`` and then
// assigns the value argument to the variable and then sets the state to
// ``full``.
//
sb.writeEF(true);

// The ``readFE()`` method blocks until the state is ``full`` and then reads
// the value of the variable, sets the state to ``empty``, and then returns the
// value.
//
var si2 = si.readFE();
writeln("si2 ", si2);

// The ``readFF()`` method blocks until the state is ``full`` and then reads
// the value of the variable and returns the value.  The state remains
// ``full``.
//
var sb2 = sb.readFF();
writeln("sb2 ", sb2);

//
// The following creates a new task via a :ref:`begin
// <primers-taskparallel-begin>` statement and declares a variable ``si4``
// that is initialized using ``si``.  The initialization statement will block
// until ``si`` is ``full``.  The last statement in the ``begin`` block sets
// ``done`` to ``full``.
//
var done: sync bool;
writeln("Launching new task");
begin {
  var si4 = si.readFE(); // This statement will block until si is full
  writeln("New task unblocked, si4=", si4);
  done.writeEF(true);
}

// Recall that execution proceeds immediately following the ``begin``
// statement after task creation.
//
writeln("After launching new task");
// When ``si`` is written, its state will be set to ``full`` and the blocked
// task above will continue.
//
si.writeEF(n);
// This next statement blocks until the last statement in the above ``begin``
// completes.
//
done.readFE();

//
// Example: simple split-phase barrier for tasks
//
var count: sync int = n;    // counter which also serves as a lock
var release: sync bool;     // barrier release

coforall t in 1..n {
  var myc = count.readFE(); // read the count, grab the lock (state = empty)
  if myc!=1 {               // still waiting for others
    write(".");
    count.writeEF(myc-1);   // update the count, release the lock (state = full)
                            // we could do some work while waiting
    release.readFF();       // wait for everyone
  } else {                  // last one here
    release.writeEF(true);  // release everyone first (state = full)
    writeln("done");
  }
}


// Sync arguments are passed by reference.  As a result, the state of the
// variable does not change.
//
si.writeEF(4*n);
writeln("now passing to f_withSyncIntFormal");
f_withSyncIntFormal(si);

// When passing a sync variable to a generic formal, whether with a ``ref``
// intent or a default intent, the variable is passed by reference. The state
// of the variable does not change and sync operations are available.
//
f_withGenericDefaultIntentFormal(si);
f_withGenericDefaultIntentFormal(sb);
f_withGenericRefFormal(si);
f_withGenericRefFormal(sb);


// Currently, sync variables cannot be written out directly.  We need to
// extract the value, for example using ``readFE()`` or ``readFF()``.
//
writeln(si.readFF());
writeln(si.readFE());

// Definitions of functions used above
proc f_withSyncIntFormal(x: sync int) {
  writeln("the value is: ", x.readFF());
}

proc f_withGenericDefaultIntentFormal(x) {
  writeln("the value is: ", x.readFF());
}

proc f_withGenericRefFormal(ref x) {
  writeln("the value is: ", x.readFF());
}
