/*
 * Multirealm Example
 *
 * This example showcases multirealm execution (running across several
 * architectural types broken into 'realms' of locales). The concepts
 * from multilocale execution apply here where the realms are simply
 * an array of the arrays of locales.
 *
 * To run this example, build Chapel and compile using the methods
 * documented in README.multirealm.  When invoking the executable, use
 * the flag "--localesPerRealm='# #...'" to specify the number of
 * locales on each realm.  For example, if you compiled your app using
 * -srealmTypes='linux darwin,' and you want to run with 2 Linux nodes
 * and 3 Macs, run "a.out --localesPerRealm='2 3'".
 */

// First, let's define our message to print from all locales:

config const message: string = "Hello, world!";

// Now let's print this message on each locale on each realm in a
// parallel way.  Note here that we're iterating over the 'Realms'
// array in the same way that we typically iterate over 'Locales' (or
// other arrays) in single-realm programs to refer to individual
// elements.  Each element r is of type 'realm' and stores a member
// array of locales named 'Locales' (which we can then traverse using
// a nested coforall).

coforall r in Realms {
  on r {
    coforall loc in r.Locales {
      on loc {

        // Note the identifier 'numRealms'.  This is a global constant
        // that stores the number of realms on which the program is
        // executing, similar to 'numLocales' in a single-realm
        // program.  In multi-realm programs like this, 'numLocales'
        // is a member of the realm type, like the Locales array.

        // You can also query the realm in which the current task is
        // executing using 'thisRealm', similar to how you can query
        // the locale with 'here' (also used in the example below).

        // Each realm supports an 'id' method that returns its unique
        // id from 0..numRealms-1.  The locales within each realm have
        // an id that is a 0-based index as well.

        var myMessage = message + " (from realm " + thisRealm.id
                      + " of " + numRealms + ", locale " + here.id
                      + " of " + thisRealm.numLocales + ")";
        writeln(myMessage);
      }
    }
  }
}
writeln();


// This prints out the Realms array, showing its member variables:

writeln("Realms is: ", Realms, "\n");


// There is also a constant sparse global domain named 'RealmSpace'
// which defines the index set for the array of Realms.  This is
// similar to the LocaleSpace domain in single-realm executions.
// The domain is sparse to exclude any realms that are defined to
// have 0 locales at execution time:

writeln("RealmSpace is: ", RealmSpace, "\n");


// Each realm also stores a string member 'rtype' representing its
// type, as defined by the -srealmTypes flag at compile-time:

for rid in RealmSpace do
  writeln("realm #", rid, " is of type: ", Realms(rid).rtype);
writeln();


// For the rest of this example, we'll refer to specific locales and
// realms, so let's ensure that we have enough before proceeding.

if (numRealms < 2 || Realms(0).numLocales < 2 || Realms(1).numLocales < 2) then
  halt("The rest of this test relies on having multiple realms and locales");


// A realm or its locales can be used in an on-clause to specify where
// a task should run.  Note that you can use the .myRealm method to
// query the realm that a given locale value belongs to.  Here we do
// this by applying .myRealm to 'here' (the task's current locale).

on Realms(0).Locales(1) do {
  writeln("On Realms(0).Locales(1)...");
  writeln("...here is ", here);
  writeln("...here.myRealm is ", here.myRealm, "\n");
}


// Using a realm as the argument to an on clause causes the task to be
// executed on its first locale.

on Realms(1) do {
  writeln("On Realms(1)...");
  writeln("...here is ", here);
  writeln("...here.myRealm is ", here.myRealm, "\n");
 }


// Because of Chapel's support for a global namespace, variables
// stored on one realm can be accessed from within another realm as
// long as the variable is within its lexical scope.  Here we declare
// x on locale 1 of realm 0 and then modify it from locale 0 of realm
// 1.  We then use the variable to do a data-driven on-clause and
// move the execution back to realm 0, locale 1.

on Realms(0).Locales(1) do {
  var x: int = 3;
  writeln("Declaring x to be ", x, " on realm 0, locale 1");
  on Realms(1).Locales(0) do {
    write("Modfiying x from realm 1, locale 0");
    x = x - 2;
    writeln(", changing its value to ", x);
    on x do {
      writeln("Using x as a data-driven on-clause...");
      writeln("...here is ", here);
      writeln("...x is ", x, "\n");
    }
  }
}


// Locales from different realms all share the same type, so it's
// possible to intermix them.  This permits, for example, a
// distribution to map a single array across multiple node types.
// Here we create an array of locales where the ith element stores
// the 0th locale from realm i.  We do this using the built-in
// domain 'RealmSpace' to describe the size of the array and the
// loop's iteration space.

var FirstLocales: [RealmSpace] locale;
for rid in RealmSpace {
  FirstLocales(rid) = Realms(rid).Locales(0);
}
writeln("The array of first locales is: ", FirstLocales, "\n");

// We can then use these locales like any other:

on FirstLocales(1) do {
  writeln("On FirstLocales(1)...");
  writeln("...here is ", here);
  writeln("...here.myRealm is ", here.myRealm, "\n");
}

// Using this same type equivalence between locales, Chapel defines
// an integer, domain, and array describing all of the locales across
// the realms of a program: totNumLocales, AllLocaleSpace,
// AllLocales:

writeln("the total number of locales is: ", totNumLocales);
writeln("the space of all locales is: ", AllLocaleSpace);
writeln("the AllLocales array contains: ", AllLocales);
writeln();


// Realm values can be captured in variables like any other type:

var rlm: realm = Realms(1);
on rlm.Locales(1) do {
  writeln("On rlm.Locales(1)...");
  writeln("...here is ", here);
  writeln("...here.myRealm is ", here.myRealm);
}
