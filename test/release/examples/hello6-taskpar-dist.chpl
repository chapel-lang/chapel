// Distributed memory task parallel hello world

/* This program shows how to use Chapel's task parallelism and
   locality features to create a concurrent "Hello, world" program
   that spans distributed memory `locales` (compute nodes) as well as
   (optionally) the processing units (cores) within each locale.  The
   number of locales on which to run is specified on the executable's
   command line using the `-nl` or `--numLocales` flag (e.g.,
   ``./hello -nl 64``).
 */


//
// The following `config const` specifies whether or not the locale
// name should be printed (for ease-of-testing purposes); the default
// can be overridden on the execution command line (e.g.,
// ``--printLocaleName=false``).
//
config const printLocaleName = true;


//
// This one specifies the number of tasks to use per locale:
//
config const tasksPerLocale = 1;

//
// Use a `coforall-loop` to create a distinct task per locale on which
// the program is executing.  Here, we're iterating over the built-in
// `Locales` array which stores an array of locale values
// corresponding 1:1 with the system's compute nodes on which the
// program is executing.  Thus, each iteration corresponds to one of
// the locales, represented by the loop index variable `loc`.
//
coforall loc in Locales {

//
// Migrate each task to its corresponding locale, `loc`.  This is done
// using an `'on'-clause`, which moves execution of the current task
// to the locale corresponding to the expression following it.
//
  on loc {

//
// Now use a second coforall-loop to create a number of tasks
// corresponding to the `tasksPerLocale` configuration constant.
// Since this loop body doesn't contain any on-clauses, all tasks will
// remain local to the current locale.
//
    coforall tid in 0..#tasksPerLocale {

//
// Start building up the message to print using a string
// variable, `message`.
//
      var message = "Hello, world! (from ";

//
// If we're running more than one task per locale, specialize the
// message based on the task ID.
//
      if (tasksPerLocale > 1) then
        message += "task " + tid:string + " of " + tasksPerLocale:string + " on ";

//
// Specialize the message based on the locale on which this task is
// running:
//
// * `here` refers to the locale on which this task is running (same as `loc`)
// * `.id` queries its unique ID in 0..`numLocales`-1
// * `.name` queries its name (similar to UNIX ``hostname``)
// * `numLocales` refers to the number of locales (as specified by -nl)
//
      message += "locale " + here.id:string + " of " + numLocales:string;

      if printLocaleName then message += " named " + loc.name;

//
// Terminate the message
//
      message += ")";

//
// Print out the message.  Since each message is being printed by a
// distinct task, they may appear in an arbitrary order.
//
      writeln(message);
    }
  }
}

//
// For further examples of using task parallelism, refer to
// :ref:`examples/primers/taskParallel.chpl <primers-taskParallel>`.
//
// For further examples of using locales and on-clauses, refer to
// :ref:`examples/primers/locales.chpl <primers-locales>`.
//
