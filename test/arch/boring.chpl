// boring.chpl

// A boring architecture
class Boring : locale
{
  proc Boring()
  {
    // Unfortunately, we don't currently have a way to call a base-class
    // constructor, so anything it does has to be copied verbatim here.
    chpl_id = __primitive("chpl_localeID");
    extern proc chpl_numCoresOnThisLocale(): int;
    numCores = chpl_numCoresOnThisLocale();
  }

  // Override the generic behavior.
  // Prints "NODE<i>" instead of "LOCALE<i>".
  proc readWriteThis(f) {
    // This prints out the actual node ID used by GASNet.
    f <~> new ioLiteral("NODE") <~> chpl_id;
  }
}

//################################################################################

// [Balance is contents of hello6-taskpar-dist.chpl, with comments stripped.]
config const printLocaleName = true;
config const tasksPerLocale = 1;

proc write_message(loc:locale, tid:int)
{
      var message = "Hello, world! (from ";

      if (tasksPerLocale > 1) then
        message += "task " + tid + " of " + tasksPerLocale + " on ";

      message += "locale " + __primitive("chpl_localeID") + " of " + numLocales;

      if (printLocaleName) then message += " named " + loc.name;

      message += ")";
      
      // writeln(loc ...) first calls loc.readWriteThis().
      writeln(loc, ": ", message);
}

//################################################################################

// 1) Show output from default locales.
// Each line should begin with "LOCALE<i>".

coforall loc in rootLocale.getLocales() do
  on loc do
    coforall tid in 0..#tasksPerLocale do
      write_message(loc, tid);


// 2) Override default locales with our own.
// Note that we can initialize the locale array in parallel!

coforall loc in rootLocale.getLocales() do
  on loc do
    rootLocale.setLocale(loc.id, new Boring());


// 3) Show output from overriding locales.
// Each line should begin with "NODE<i>".

coforall loc in rootLocale.getLocales() do
  on loc do
    coforall tid in 0..#tasksPerLocale do
      write_message(loc, tid);


writeln("Done.");

