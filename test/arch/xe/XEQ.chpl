// XEQ.chpl
//
// Architectural description for XE machines, using qthreads.
//

config const debugArchitecture = false;

extern proc chpl_task_getNumSheps() : int(32);
config const numShepherds = chpl_task_getNumSheps();
writeln("numShepherds = ", numShepherds);

// libhwloc may be more helpful here.
// These numbers are taken from marketing documentation.
// Remember to run with aprun -d 16, so you get the IL16 compute nodes.

// Each socket has four DDR3 1600 memory channels, for a possible peak bandwidth
// of 51.2 GB/s (102.4 GB/s per node)
const il_sockets_per_node = 2;
const il_dice_per_socket = 2;
// Each die contains four cores, a memory controller and an 8MB L3 cache.
// Each core has private L1 and L2 caches.
const il_cores_per_die = 4;
// Interlagos cores are hyperthreaded, so they actually look like two processing elements.
const il_pes_per_core = 2;

// Or, to save on postage....
extern proc chpl_numCoresOnThisLocale(): int;

// An XEPart is a part of the whole XE node.
// Current definition is to have one part per shepherd
// Qthreads apparently thinks it should use two shepherds per core, so that's what we get by default.
class XEPart : locale
{
  var my_shepherd_id : int(32);

  proc XEPart()
  {
    // Base-class initializer call.
    chpl_id = __primitive("chpl_localeID");
    numCores = chpl_numCoresOnThisLocale() / numShepherds;
  }

  proc initTask() : void
  {
    extern proc qthread_migrate_to(shep:int(32)) : int;
    qthread_migrate_to(my_shepherd_id);
  }

  proc readWriteThis(f) {
    f <~> new ioLiteral("XEPart ") <~> my_shepherd_id;
  }
}

class XENode : locale
{
  var sublocDom : domain(int);
  var subLocales : [sublocDom] locale;

  proc XENode()
  {
    chpl_id = __primitive("chpl_localeID");
    numCores = chpl_numCoresOnThisLocale();
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
    var part = child : XEPart;
    // The shepherd ID is zero-based.
    part.my_shepherd_id = my_id - 1;
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

  iter getChildren() : locale
  {
    for idx in sublocDom do
      yield subLocales[idx];
  }

  proc getChildCount() : int
  { return sublocDom.numIndices; }
}

//################################################################################

// Override default locales with our own.
// Note that we can initialize the locale array in parallel!

coforall loc in rootLocale.getLocales() do
  on loc {
    rootLocale.setLocale(loc.id, new XENode());
  }

// Initialize the sublocales on each locale.
coforall loc in rootLocale.getLocales() do
  on loc {
    for idx in {1..numShepherds} do
      on __primitive("chpl_on_locale_num", loc.id, idx)
      {
        // The subloc index is one-based, but the ID is zero-based.
        var my_subloc = new XEPart(idx-1);
        here.addChild(my_subloc);
      }
  }

writeln("Done with XEQ setup.");

if (debugArchitecture) then 
  for loc in rootLocale.getLocales() {
    writeln("XENode: ", loc);
    for subloc in (here:XENode).getChildren() do
      writeln("  XEPart: ", subloc);
  }

