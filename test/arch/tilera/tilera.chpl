// tilera.chpl
//

extern type cpu_set_t;
extern proc tmc_cpus_get_online_cpus(inout cpus:cpu_set_t): int;
extern proc tmc_cpus_count(inout cpus:cpu_set_t): int;

// A TileraPart is a partition of the whole Tilera chip.
class TileraPart : locale
{
  var my_cpus : cpu_set_t;
  var subloc_id : int;

  // This constructor takes a partition description string
  // and uses it to initialize the CPU set available to this partition.
  proc TileraPart(part_desc:string)
  {
    // Tilera();	// Base-class initializer call.
    chpl_id = __primitive("chpl_localeID");
    var cpus : cpu_set_t;
    numCores = 1;
    if tmc_cpus_get_online_cpus(cpus) == 0 then
      numCores = tmc_cpus_count(cpus);
    
    extern proc tmc_cpus_from_string(inout cpus:cpu_set_t, s:string):int;
    if tmc_cpus_from_string(my_cpus, part_desc) != 0 then
      halt("Problem converting Tilera CPU descriptor string to type cpu_set_t.");
  }

  proc initTask() : void
  {
    extern proc tmc_cpus_set_my_affinity(inout cpus:cpu_set_t) : int;
    if tmc_cpus_set_my_affinity(my_cpus) != 0 then
      halt("Problem setting CPU affinity for this task.");
  }

  proc readWriteThis(f) {
    extern proc tmc_cpus_get_my_current_cpu() : int;
    f <~> new ioLiteral("TileraPart ") <~> subloc_id <~> " CPU " <~> tmc_cpus_get_my_current_cpu();
  }
}

class Tilera : locale
{
  var sublocCount = 0;
  var sublocDom : domain(int);
  var subLocales : [sublocDom] locale;

  proc Tilera()
  {
    chpl_id = __primitive("chpl_localeID");
    var cpus : cpu_set_t;
    numCores = 1;
    if tmc_cpus_get_online_cpus(cpus) == 0 then
      numCores = tmc_cpus_count(cpus);
  }

  // Override the generic behavior.
  // Prints "NODE<i>" instead of "LOCALE<i>".
  proc readWriteThis(f) {
    // This prints out the actual node ID used by GASNet.
    f <~> new ioLiteral("NODE") <~> chpl_id;
  }

  proc addChild(child : locale) : void
  {
    sublocCount += 1;
    sublocDom.add(sublocCount);
    subLocales[sublocCount] = child;
    var dcChild = child : TileraPart;
    dcChild.subloc_id = sublocCount;
  }

  proc getChild(subloc_id:int) : locale
  {
    if subloc_id > 0 && subloc_id <= sublocCount then
      return subLocales[subloc_id];
    return this;
  }
}

//################################################################################

// [Balance is contents of hello6-taskpar-dist.chpl, with comments stripped.]
config const printLocaleName = true;

proc write_message(loc:locale)
{
      var message = "Hello, world! (from ";

      message += "locale " + __primitive("chpl_localeID") + " of " + numLocales;

      if (printLocaleName) then message += " named " + loc.name;

      message += " having " + loc.numCores + " online cores";

      message += ")";
      
      // writeln(loc ...) first calls loc.readWriteThis().
      writeln(loc, ": ", message);
}

//################################################################################

// Override default locales with our own.
// Note that we can initialize the locale array in parallel!

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

