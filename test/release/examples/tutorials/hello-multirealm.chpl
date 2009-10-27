/*
 * Multirealm Example
 *
 * This example showcases multirealm execution (running across several
 * architectural types broken into 'realms' of locales). The concepts
 * from multilocale execution apply here where a realm is simply an array
 * of the arrays of locales.
 *
 * To run this example, build Chapel and compile using the methods
 * documented in README.multirealm. Use the "--localesPerRealm='# #...'"
 * flag to specify the number of locales on each realm. For example, if
 * you compiled your app using -srealmTypes='linux darwin,' and you
 * want to run with 2 Linux nodes and 3 Macs, run
 * "a.out --localesPerRealm='2 3'".
 */

// First, let's define our hello world string to send across all locales:

config var message: string = "Hello, world!";

// Now let's print this message on each locale on each realm in a parallel
// way. Note here that we're treating Realms as an array to traverse over
// in the same way we used (and use here, as well) Locales to grab
// individual locales. The r in Realms is an array of locales (which we
// can then coforall over).

coforall r in Realms {
  on r {
    coforall loc in r.Locales {
      on loc {

        // Note the constant numRealms. This can be accessed anywhere in
        // the same way numLocales can be (and is here, as well).

        // You can also query the realm using thisRealm in the same way
        // you can query the locale with here (also in example below).

        var myMessage = message + " (from realm " + thisRealm.id
                      + " of " + numRealms + ", locale " + here.id
                      + " of " + thisRealm.numLocales + ")";
        writeln(myMessage);
      }
    }
  }
}

// This explicitly shows us the Realms array.

writeln("Realms is: ", Realms);

// One way to get the total number of locales over all realms would be
// to use the RealmSpace array, which acts like the LocaleSpace array.
// The RealmSpace array is just a set of indices from 0 to numRealms-1.

var totalLocales: int = + reduce ([i in RealmSpace] Realms[i].numLocales);
writeln("Using RealmSpace to iterate over Realms array");
writeln("Total number of locales: ", totalLocales);

// The realm type is really a record with numLocales, LocaleSpace, and
// the Locales array. Note you can use the .myRealm member to query the
// realm you're on.

on Realms(0).Locales(1) do {
  writeln("Example of using Realm members");
  writeln("here is ", here.id);
  writeln("myRealm is ", here.myRealm);
}

// We can access an variable like we access it using locales.
on Realms(0).Locales(1) do {
  var x: int = 3;
  on Realms(0).Locales(0) do {
    x = x - 2;
    on x do {
      writeln("Query by data variable");
      writeln("here is ", here.id);
      writeln("x is ", x);
    }
  }
}

// Flattening a realms array into all nodes. Note the use of the # operator
// to say go from i to the numLocales in that realm.

var i: int = 0;
var MyLocales: [0..#totalLocales] locale;  // uses totalLocales from above
for r in Realms {
  MyLocales(i..#r.numLocales) = r.Locales;
  i += r.numLocales;
}
writeln("Flattening the realms array into one single array of locales");
writeln(MyLocales);
on MyLocales(1) do {
  writeln("here.id is ", here.id);
  writeln("here.myRealm is ", here.myRealm);
}

// Using realms as a variable:
var rlm: realm = Realms(0);
on rlm.Locales(0) do {
  writeln("Realm as variable");
  writeln("here.myRealm is ", here.myRealm);
}
