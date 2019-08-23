// userSync.chpl
//
// A very clever user can elide reference counting on a shared variable (using
// the "no copy" and "no auto destroy" pragmas if he knows that the spawning
// task will not attempt to delete a shared variable before every spawned task
// is done with it.  
// For example, inside a coforall, protection of a shared variable is not
// necessary, because all those child tasks must terminate before the coforall
// exits.  
//

  extern const chpl_nodeID: chpl_nodeID_t;
  extern const c_sublocid_any: chpl_sublocID_t;

  iter visitLocales() {
    for locIdx in 0..#numLocales {
      yield locIdx;
    }
  }

  iter visitLocales(param tag: iterKind)
    where tag==iterKind.leader {
    // Simple locales barrier, see implementation below for notes
    var b: localesBarrier;
    pragma "no auto destroy"
    var flags: [1..#numLocales-1] unmanaged localesSignal?;
    coforall locIdx in 0..#numLocales /*ref(b)*/ {
      on __primitive("chpl_on_locale_num",
                     chpl_buildLocaleID(locIdx:chpl_nodeID_t,
                                        c_sublocid_any)) {
        yield locIdx;
        b.wait(locIdx, flags);
      }
    }
  }

  iter visitLocales(param tag: iterKind, followThis)
    where tag==iterKind.follower {
    yield followThis;
  }

  use CommDiagnostics;

  proc init() {
    startCommDiagnostics();
    forall locIdx in visitLocales() {
      //      writeln("On locale ", locIdx);
      writeln("Called visitLocales().");
    }
    stopCommDiagnostics();
    writeln(getCommDiagnostics());
  }

  init();

