// tilera.chpl
//

extern type cpu_set_t;
extern proc tmc_cpus_get_online_cpus(inout cpus:cpu_set_t): int;
extern proc tmc_cpus_count(inout cpus:cpu_set_t): int;

// A TileraPart is a partition of the whole Tilera chip.
class TileraPart : locale
{
  var my_cpus : cpu_set_t;
  var my_subloc_id : int;

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

  proc TileraPart(cpu: int)
  {
    // Tilera();	// Base-class initializer call.
    chpl_id = __primitive("chpl_localeID");
    var cpus : cpu_set_t;
    numCores = 1;
    if tmc_cpus_get_online_cpus(cpus) == 0 then
      numCores = tmc_cpus_count(cpus);

    extern proc tmc_cpus_clear(inout cpus:cpu_set_t) : void;
    extern proc tmc_cpus_add_cpu(inout cpus:cpu_set_t, cpu:int(32)) : int;
    tmc_cpus_clear(my_cpus);
    if tmc_cpus_add_cpu(my_cpus, cpu:int(32)) != 0 then
      halt("Could not add to cpu set.");
  }

  proc initTask() : void
  {
    extern proc tmc_cpus_set_my_affinity(inout cpus:cpu_set_t) : int;
    if tmc_cpus_set_my_affinity(my_cpus) != 0 then
      halt("Problem setting CPU affinity for this task.");
  }

  proc readWriteThis(f) {
    extern proc tmc_cpus_get_my_current_cpu() : int;
    f <~> new ioLiteral("TileraPart ") <~> my_subloc_id <~> " CPU " <~> tmc_cpus_get_my_current_cpu();
  }
}

class Tilera : locale
{
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
    var my_id = __primitive("_wide_get_subloc", child);
    sublocDom.add(my_id);
    subLocales[my_id] = child;
    var dcChild = child : TileraPart;
    dcChild.my_subloc_id = my_id;
  }

  proc getChild(subloc_id:int) : locale
  {
    // This code is sensitive to initialization ordering --
    // can be called before here is initialized.
    // The subloc_id > 0 test lets us slide past that (though we'd like a better solution).
    if subloc_id > 0 && sublocDom.member(subloc_id) then
      return subLocales[subloc_id];
    return this;
  }
}

//################################################################################

// Override default locales with our own.
// Note that we can initialize the locale array in parallel!

coforall loc in rootLocale.getLocales() do
  on loc do
    rootLocale.setLocale(loc.id, new Tilera());

// var my_subloc : locale = nil;

extern proc chpl_task_getSubLoc() : int(32);

for idx in {1..36} do
  on __primitive("chpl_on_locale_num", 0, idx)
  {
    var my_subloc = new TileraPart(idx % 36);
    here.addChild(my_subloc);
  }

coforall idx in {1..36}
{
  var loc = here.getChild(idx);
  on loc do
    writeln(loc);
}

writeln("Done.");

