use CommUtil;
use CyclicDist;

config const numElements = 1024;

const cyclicSpace = {0..#numElements};
const cyclicDom = cyclicSpace dmapped Cyclic(startIdx=0);
var cyclicArr : [cyclicDom] int;

writeln("Querying via domain distribution...");
start();
coforall loc in Locales do on loc {
  coforall tid in 1..here.maxTaskPar {
    for i in cyclicSpace {
      var locid = cyclicDom.dist.idxToLocale(i).id;
    }
  }
}
stop();
report(maxGets=0,
       maxOns = if CHPL_NETWORK_ATOMICS == 'none' then 2 else 1);

writeln("Querying via array reference...");
start();
coforall loc in Locales do on loc {
  coforall tid in 1..here.maxTaskPar {
    for i in cyclicSpace {
      var locid = cyclicArr[i].locale.id;
    }
  }
}
stop();
report(maxGets=14,
       maxOns = if CHPL_NETWORK_ATOMICS == 'none' then 2 else 1);

writeln("Querying via wide pointer...");
start();
coforall loc in Locales do on loc {
  coforall tid in 1..here.maxTaskPar {
    for i in cyclicSpace {
      var loc = cyclicDom.dist.idxToLocale(i);
      var locID = chpl_nodeFromLocaleID(__primitive("_wide_get_locale", loc));
      assert(cyclicArr[i].locale == Locales[locID]);
    }
  }
}
stop();
report(maxGets=0,
       maxOns = if CHPL_NETWORK_ATOMICS == 'none' then 2 else 1);

inline proc getLocale(dom, idx) {
  var loc = dom.dist.idxToLocale(idx);
  var locID = chpl_nodeFromLocaleID(__primitive("_wide_get_locale", loc));

  // Handles cases where we get a locale that is allocated on another locale...
  if locID == here.id then return loc;
  else return Locales[locID];
}

writeln("Querying via wide pointer inside function...");
start();
coforall loc in Locales do on loc {
  coforall tid in 1..here.maxTaskPar {
    for i in cyclicSpace {
      var locid = getLocale(cyclicDom, i).id;
    }
  }
}
stop();
report(maxGets=0,
       maxOns = if CHPL_NETWORK_ATOMICS == 'none' then 2 else 1);

writeln("Querying remote object...");
var obj = new object();
start();
coforall loc in Locales do on loc {
  coforall tid in 1..here.maxTaskPar {
    for i in cyclicSpace {
      var locid = obj.locale.id;
    }
  }
}
stop();
report(maxGets=0,
       maxOns = if CHPL_NETWORK_ATOMICS == 'none' then 2 else 1);

