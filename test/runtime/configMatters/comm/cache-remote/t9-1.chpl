// various utilites for testing
use IO;

extern proc chpl_cache_print();
pragma "insert line file info"
config const verbose=false;


/// label the output with "phases" //////////////////////////////////////////

var pfx = "# ";
var phase = 0;
var phase2 = 0;

proc nextphase() {
  phase += 1;
  phase2 = phase;
  pfx = "%{####}".format(phase) + "  ";
}

// heading/start new "phase"
proc hd(args...) { nextphase(); writeln(phase, phase2, pfx, (...args)); }
proc hhd(args...) { nextphase(); writeln(phase, phase2, pfx, (...args), " on ", here.id); }
proc msg(args...) { writeln(phase, phase2, pfx, (...args)); }

for l in Locales do on l {
  if l != Locales(0) {
    doShowDom(LocaleSpace);
  }
}

proc doShowDom(dmdom) {
  hhd("A");
  for i in LocaleSpace {
    msg(" ", i, "  on ", here.id);
  }

  hhd("B");
  for i in LocaleSpace {
    msg(" ", i, "  on ", here.id);
  }
}



