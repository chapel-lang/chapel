//
// Sync and Single Variables Primer
//
// This primer illustrates Chapel's sync and single variables.
//
config const n = 7;

// Sync and single are type qualifiers that can be applied to all
// Chapel primitive types except strings, complex, and record.
//

// Sync and single variables have an associated state that is either
// 'full' or 'empty'.  Single variables are sync variables that can
// only be written once.  If a sync or single variable is declared
// with an initializing expression, its state is set to full and its
// value is that of the expression.  Without an initializing
// expression, a sync or single variable's state is set to empty and
// its value is the default value for the base type.
//
var sy$: sync int=1;  // state = full, value = 1
var si$: single bool; // state = empty, value = false

// By convention, sync and single variable names end in a $ to
// add a visual cue indicating that reads and writes are possibly
// expensive operations and can potentially block the task.
//

// For both sync and single variables, the state must be empty before
// the value can be written.  When the write has completed, the state
// is set to full.
//
si$ = true; // state = full, value = true

// For both sync and single variables, the state must be full before the
// value can be read.  When the read has completed, the state of a
// sync variable is set to empty, and the state of a single variable
// remains full.
//
var sy = sy$; // sy$: state = empty
var si = si$; // si$: state = full

// If a sync or single variable is not in the correct state for reading
// (full) or writing (empty), the current task blocks.
//
// The following creates a new task via a begin statement and declares
// a variable sy that is initialized to sy$.  The initialization statement
// will block until sy$ is full.  The last statement in the begin
// block sets the done$ to full.
//
var done$: sync bool;
writeln("Launching new task");
begin {
  var sy = sy$; // This statement will block until sy$ is full
  writeln("New task unblocked, sy=", sy);
  done$ = true;
}

// Recall that execution proceeds immediately following the begin
// statement after task creation.
//
writeln("After launching new task");
// When sy$ is written, its state will be set to full and the blocked
// task above will continue.
//
sy$ = n;
// This next statement blocks until last statement in the above begin
// completes.
//
done$;

//
// Example: simple split-phase barrier for tasks
//
var count$: sync int = n;  // counter which also serves as a lock
var release$: single bool; // barrier release

coforall t in 1..n {
  var myc = count$;   // read the count, grab the lock (state = empty)
  if myc!=1 {         // still waiting for others
    write(".");
    count$ = myc-1;   // update the count, release the lock (state = full)
                      // we could do some work while waiting
    release$;         // wait for everyone
  } else {            // last one here
    release$ = true;  // release everyone first (state = full)
    writeln("done");
  }
}

//
// There are a number of methods defined for sync and single variables.
//

// The reset() method, defined for sync variables, sets the value of
// the variable to the default value for the type and the state to
// empty.
//
sy$.reset();  // value = 0, state = empty

// The isFull method returns true if the sync or single variable is
// in the full state, false otherwise.
//
writeln(sy$.isFull);
writeln(si$.isFull);

// The writeEF() method, defined for sync and single variables,
// blocks until the state is empty and then assigns the value argument
// to the variable and then sets the state to full.  Assignment of
// sync and single variables are performed using writeEF().
//
sy$.writeEF(2*n); // equivalent to: sy$ = 2*n;

// The readFE() method, defined for sync variables, blocks until the
// state is full and then reads the value of the variable, sets the
// state to empty, and then returns the value.  Normal reads of sync
// variables are performed using readFE().
//
var sy2 = sy$.readFE(); // equivalent to: var sy2 = sy$;
writeln(sy2);

// The readFF() method, defined for sync and single variables, blocks
// until the state is full and then reads the value of the variable
// and returns the value.  The state remains full.  Normal reads of
// single variables are performed using readFF().
//
var si2 = si$.readFF();
writeln(si2);

// The writeXF() method, defined for sync variables, assigns the
// value argument to the variable and then sets the state to full.
// This method does not block.
//
sy$.writeXF(3*n);

// The readXX() method, defined for sync and single variables, returns
// the value of the variable regardless of the state.  This method
// does not block and the state is unchanged.
//
var sy3 = sy$.readXX();
var si3 = si$.readXX();
writeln(sy3);
writeln(si3);

// The writeFF() method, defined for sync variables, blocks until the
// state is full and then and then assigns the value argument to the
// variable.  The state is unchanged.
//
sy$.writeFF(4*n);

sy$.reset();

// Sync and single arguments are passed by reference.  As a result,
// the state of the variable does not change.
//
writeln(sy$.isFull);
f_withSyncIntFormal(sy$);
writeln(si$.isFull);
f_withSingleBoolFormal(si$);

sy$ = 4*n;

// When a sync or single variable is passed as an argument to a
// function that expects the base type of the variable, the value is
// read before being passed to the function.  Therefore, the task will
// block until the state of the variable is full.
//
f_withIntFormal(sy$);
f_withBoolFormal(si$);

// When passing a sync or single variable to a generic formal,
// whether with a ref intent or a blank intent, the variable
// is passed by reference. The state of the variable does not
// change and sync operations are available.
//
f_withGenericBlankIntentFormal(sy$);
f_withGenericBlankIntentFormal(si$);
f_withGenericRefFormal(sy$);
f_withGenericRefFormal(si$);

sy$ = 5*n;

// Currently, sync and single variables cannot be written out directly.
// We need to extract the value, for example using readFE() or readFF().
//
writeln(sy$.readFE());
writeln(si$.readFF());

proc f_withSyncIntFormal(x: sync int) {
  writeln(x.isFull);
}

proc f_withSingleBoolFormal(x: single bool) {
  writeln(x.isFull);
}

proc f_withIntFormal(x: int) {
  writeln(x);
}

proc f_withBoolFormal(x: bool) {
  writeln(x);
}

proc f_withGenericBlankIntentFormal(x) {
  writeln("the full bit is: ", x.isFull);
}

proc f_withGenericRefFormal(ref x) {
  writeln("readXX returns: ", x.readXX());
}
