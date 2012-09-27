// tilera-driver.chpl
use tilera;

// Override default locales with our own.
// Note that we can initialize the locale array in parallel!
proc main ()
{
  // Populate the entire cluster with Tilera nodes.
  coforall loc in rootLocale.getLocales() do
    on loc do
      rootLocale.setLocale(loc.id, new Tilera());

  var my_subloc : locale = nil;

  extern proc chpl_task_getSubLoc() : int(32);

  on __primitive("chpl_on_locale_num", 0, 1)
  {
    my_subloc = new TileraPart("12-15");
    writeln("my_subloc thinks it is on sublocale ",
            __primitive("_wide_get_subloc", my_subloc));
    here.addChild(my_subloc);
  }

  on my_subloc
  {
    writeln(my_subloc);
  }


  writeln("Done.");
}