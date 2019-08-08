use CyclicDist;
use BlockDist;
use Random;
use Futures;
use CommDiagnostics;
use VisualDebug;
use Memory;

// Iterators for getting around issue with reduction on explicit 'coforall'
iter forEachLocale() : int { halt("Serial iterator not implemented!"); }
iter forEachLocale(param tag : iterKind) : int where tag == iterKind.standalone {
  coforall loc in Locales do on loc do yield here.id;
}

iter forEachCorePerLocale() : int { halt("Serial iterator not implemented!"); }
iter forEachCorePerLocale(param tag : iterKind) : int where tag == iterKind.standalone {
  coforall loc in Locales do on loc {
    coforall tid in 0..#here.maxTaskPar {
      yield (here.id, tid);
    }
  }
}

config const printLocaleStatistics = false;
if printLocaleStatistics {
  var memory : [LocaleSpace] int;
  var maxTaskPar : [LocaleSpace] int;
  coforall loc in Locales do on loc {
    memory[here.id] = here.physicalMemory();
    maxTaskPar[here.id] = here.maxTaskPar;
  }
  writeln("Memory Availalbe: ", memory);
  writeln("Cores Available: ", maxTaskPar);
}

pragma "no doc"
pragma "default intent is ref"
record Lock {
  var _lock : chpl__processorAtomicType(bool);

  inline proc acquire() {
    on this do local {
      if _lock.testAndSet() == true { 
        while _lock.read() == true || _lock.testAndSet() == true {
          chpl_task_yield();
        }
      }
    }
  }

  inline proc release() {
    on this do local do _lock.clear();
  }
}

pragma "no doc"
// Acquires in a global lock order to ensure we do not deadlock
proc acquireLocks(ref a : Lock, ref b : Lock) {
  if a.locale.id > b.locale.id || __primitive("cast", uint(64), __primitive("_wide_get_addr", a)) > __primitive("cast", uint(64), __primitive("_wide_get_addr", b)) {
    if a.locale != here && b.locale != here && a.locale == b.locale {
      on a {
        a.acquire();
        b.acquire();
      }
    } else {
      a.acquire();
      b.acquire();
    }
  } else {
    if a.locale != here && b.locale != here && a.locale == b.locale {
      on b {
        b.acquire();
        a.acquire();
      }
    } else {
      b.acquire();
      a.acquire();
    }
  }
}

pragma "no doc"
// Releases in global locking order so that we do not deadlock
proc releaseLocks(ref a : Lock, ref b : Lock) {
  if a.locale.id > b.locale.id || __primitive("cast", uint(64), __primitive("_wide_get_addr", a)) > __primitive("cast", uint(64), __primitive("_wide_get_addr", b)) {
    if a.locale != here && b.locale != here && a.locale == b.locale {
      on a {
        a.release();
        b.release();
      }
    } else {
      a.release();
      b.release();
    }
  } else {
    if a.locale != here && b.locale != here && a.locale == b.locale {
      on b {
        b.release();
        a.release();
      }
    } else {
      b.release();
      a.release();
    }
  }
}

config param debugMode = false;
config const profileCommDiagnostics = false;
config const profileCommDiagnosticsVerbose = false;
config const profileVisualDebug = false;

/*
  Only prints if `printDebugInformation` is enabled; warning: Chapel still evaluates arguments
  so side-effects will still occur!
*/
proc debug(args...) where debugMode {
  writeln((...args));
}

proc debug(args...) where !debugMode {}

proc beginProfile(vdebugName = "vdebug") {
  if profileCommDiagnostics {
    startCommDiagnostics();
  }
  if profileCommDiagnosticsVerbose {
    startVerboseComm();
  }
  if profileVisualDebug {
    startVdebug(vdebugName);
  }
}

proc endProfile() {
  if profileCommDiagnosticsVerbose {
    stopVerboseComm();
  }
  if profileCommDiagnostics {
    stopCommDiagnostics();
    for (loc, diag) in zip(Locales, getCommDiagnostics()) {
      writeln(loc, ": ", diag);
    }
  }
  if profileVisualDebug {
    stopVdebug();
  }
}

/*
  Array reference wrapper; captures the privatization id and instance, and can be treated mostly
  as an array, except without the implicit by-value copying. Note that the lifetime of the original
  array is unaffected by this 'reference', hence care must be used to not use this after the original 
  array goes out of scope.
*/
pragma "always RVF"
record ArrayRef {
  var pid : int = -1;
  var instance;

  proc init(ref A : []) {
    this.pid = A._pid;
    this.instance = A._instance;
  }

  proc _value {
    if boundsChecking && ((_isPrivatized(instance) && pid == -1) || instance == nil) {
      halt("Attempt to use ArrayRef without initialization!");
    }
    
    if _isPrivatized(instance) {
      return chpl_getPrivatizedCopy(instance.type, pid);
    } else {
      return instance;
    }
  }
  
  forwarding _value;
}

pragma "no doc"
/*
  Checks to see if the array has a local array and domain.
*/
proc isLocalArray(A : []) : bool {
  return A.locale == here && A._value.locale == here && A._value.dom.locale == here && A.domain._value.locale == here;
}

/*
  Obtains the intersection of two arrays, A and B. This method is optimized for
  locality and will copy any remote arrays to be entirely local; this includes
  a locality check for the array and the domain itself (which will be remote if
  the user creates an implicit copy).
*/
proc intersection(A : [] ?t, B : [] t) {
  if isLocalArray(A) && isLocalArray(B) {
    return _intersection(A, B);
  } else if isLocalArray(A) && !isLocalArray(B) {
    const _BD = B.domain; // Make by-value copy so domain is not remote.
    var _B : [_BD] t = B;
    return _intersection(A, _B);
  } else if !isLocalArray(A) && isLocalArray(B) {
    const _AD = A.domain; // Make by-value copy so domain is not remote.
    var _A : [_AD] t = A;
    return _intersection(_A, B);
  } else {
    const _AD = A.domain; // Make by-value copy so domain is not remote.
    const _BD = B.domain;
    var _A : [_AD] t = A;
    var _B : [_BD] t = B;
    return _intersection(_A, _B);
  }
}

pragma "no doc"
proc _intersection(A : [] ?t, B : [] t) {
  var CD = {0..#min(A.size, B.size)};
  var C : [CD] t;
  local {
    var idxA = A.domain.low;
    var idxB = B.domain.low;
    var idxC = 0;
    while idxA <= A.domain.high && idxB <= B.domain.high {
      const a = A[idxA];
      const b = B[idxB];
      if a == b { 
        C[idxC] = a;
        idxC += 1;
        idxA += 1; 
        idxB += 1; 
      }
      else if a > b { 
        idxB += 1;
      } else { 
        idxA += 1;
      }
    }
    CD = {0..#idxC};
  }
  return C;
}

/*
  Obtains the intersection size of two arrays, A and B. This method is optimized for
  locality and will copy any remote arrays to be entirely local; this includes
  a locality check for the array and the domain itself (which will be remote if
  the user creates an implicit copy).
*/
proc intersectionSize(A : [] ?t, B : [] t) {
  if isLocalArray(A) && isLocalArray(B) {
    return _intersectionSize(A, B);
  } else if isLocalArray(A) && !isLocalArray(B) {
    const _BD = B.domain; // Make by-value copy so domain is not remote.
    var _B : [_BD] t = B;
    return _intersectionSize(A, _B);
  } else if !isLocalArray(A) && isLocalArray(B) {
    const _AD = A.domain; // Make by-value copy so domain is not remote.
    var _A : [_AD] t = A;
    return _intersectionSize(_A, B);
  } else {
    const _AD = A.domain; // Make by-value copy so domain is not remote.
    const _BD = B.domain;
    var _A : [_AD] t = A;
    var _B : [_BD] t = B;
    return _intersectionSize(_A, _B);
  }
}

pragma "no doc"
proc _intersectionSize(A : [] ?t, B : [] t) {
  var match : int;
  local {
    var idxA = A.domain.low;
    var idxB = B.domain.low;
    while idxA <= A.domain.high && idxB <= B.domain.high {
      const a = A[idxA];
      const b = B[idxB];
      if a == b { 
        match += 1;
        idxA += 1; 
        idxB += 1; 
      }
      else if a > b { 
        idxB += 1;
      } else { 
        idxA += 1;
      }
    }
  }
  return match;
}

/*
  Determines whether the size of the intersection of two arrays, A and B is at least 's'. 
  This method is optimized for locality and will copy any remote arrays to be entirely local; 
  this includes a locality check for the array and the domain itself (which will be remote if
  the user creates an implicit copy).
*/
proc intersectionSizeAtLeast(A : [] ?t, B : [] t, s : integral) {
  if isLocalArray(A) && isLocalArray(B) {
    return _intersectionSizeAtLeast(A, B, s);
  } else if isLocalArray(A) && !isLocalArray(B) {
    const _BD = B.domain; // Make by-value copy so domain is not remote.
    var _B : [_BD] t = B;
    return _intersectionSizeAtLeast(A, _B, s);
  } else if !isLocalArray(A) && isLocalArray(B) {
    const _AD = A.domain; // Make by-value copy so domain is not remote.
    var _A : [_AD] t = A;
    return _intersectionSizeAtLeast(_A, B, s);
  } else {
    const _AD = A.domain; // Make by-value copy so domain is not remote.
    const _BD = B.domain;
    var _A : [_AD] t = A;
    var _B : [_BD] t = B;
    return _intersectionSizeAtLeast(_A, _B, s);
  }
}


// Checks to see if they have at least 's' in common
proc _intersectionSizeAtLeast(A : [] ?t, B : [] t, s : integral) {
  if s == 0 then return true;
  var match : int;
  local {
    var idxA = A.domain.low;
    var idxB = B.domain.low;
    while idxA <= A.domain.high && idxB <= B.domain.high {
      const a = A[idxA];
      const b = B[idxB];
      if a == b { 
        match += 1;
        if match >= s then break;
        idxA += 1; 
        idxB += 1; 
      }
      else if a > b { 
        idxB += 1;
      } else { 
        idxA += 1;
      }
    }
  }
  return match >= s;
}

proc _arrayEquality(A : [] ?t, B : [] t) {
  return A.equals(B);
}

proc arrayEquality(A : [] ?t, B : [] t) {
  if isLocalArray(A) && isLocalArray(B) {
    return _arrayEquality(A, B);
  } else if isLocalArray(A) && !isLocalArray(B) {
    const _BD = B.domain; // Make by-value copy so domain is not remote.
    var _B : [_BD] t = B;
    return _arrayEquality(A, _B);
  } else if !isLocalArray(A) && isLocalArray(B) {
    const _AD = A.domain; // Make by-value copy so domain is not remote.
    var _A : [_AD] t = A;
    return _arrayEquality(_A, B);
  } else {
    const _AD = A.domain; // Make by-value copy so domain is not remote.
    const _BD = B.domain;
    var _A : [_AD] t = A;
    var _B : [_BD] t = B;
    return _arrayEquality(_A, _B);
  }
}

extern type chpl_comm_nb_handle_t;

extern proc chpl_comm_get_nb(
    addr : c_void_ptr, node : chpl_nodeID_t, raddr : c_void_ptr, 
    size : size_t, typeIndex : int(32), commID : int(32), 
    ln : c_int, fn : int(32)
) : chpl_comm_nb_handle_t;

inline proc getAddr(ref x : ?t) : c_void_ptr {
  return __primitive("_wide_get_addr", x);
}

inline proc getLocaleID(ref x : ?t) : chpl_localeID_t {
  return __primitive("_wide_get_locale", x);
}

inline proc getNodeID(ref x : ?t) : chpl_nodeID_t {
  return chpl_nodeFromLocaleID(getLocaleID(x));
}

proc get_nb(ref r1 : ?t1) : Future((t1,)) {
  record FutureCallback1 {
    type _t1;
    var _r1 : _t1; 
    var h1 : chpl_comm_nb_handle_t;

    proc init(type _t1, ref r1 : _t1) {
      this._t1 = _t1;
      // TODO: Find a way to typeIndex
      //chpl_comm_get_nb(getAddr(r1), getNodeID(r1), getAddr(_r1), sizeof(_t1), 
    }

    proc this() {
      // TODO
    }
  }
}


// Random Number Generator utilities...
var _globalIntRandomStream = makeRandomStream(int, parSafe=true);
var _globalRealRandomStream = makeRandomStream(real, parSafe=true);

proc randInt(low, high) {
  return _globalIntRandomStream.getNext(low, high);
}

proc randInt(high) {
  return randInt(0, high);
}

proc randInt() {
  return randInt(min(int), max(int));
}

proc randReal(low, high) {
  return _globalRealRandomStream.getNext(low, high);
}

proc randReal(high) {
  return randReal(0, high);
}

proc randReal() {
  return randReal(0, 1);
}

// Obtain random element from an associative domain
proc getRandomAssociative(dom : domain, ref elt : dom.eltType) : bool {
  if dom.size == 0 then return false;
	// Get random index...
	var idx = randInt(0, dom.table.size - 1);
	while true {
		if dom.table[idx].status == chpl__hash_status.empty {
			idx += 1;
			idx = idx % dom.table.size;
			continue;
		}
		elt = dom.table[idx].idx;
    return true;
	}
	return false;
}

// Utilize the fact that a 'class' in Chapel is a heap-allocated object, and all
// remote accesses will have to go through the host first, hence making it centralized.
class Centralized {
  var x;
  proc init(x) {
    this.x = x;
  }

  proc init(type X) {
    this.x = new X();
  }

  forwarding x;
}

inline proc getLocaleIdx(dom, idx) {
  var loc = dom.dist.idxToLocale(idx);
  var locID = chpl_nodeFromLocaleID(__primitive("_wide_get_locale", loc));
  return locID;
}

inline proc getLocale(dom, idx) {
  var loc = dom.dist.idxToLocale(idx);
  var locID = chpl_nodeFromLocaleID(__primitive("_wide_get_locale", loc));
  
  // Handles cases where we get a locale that is allocated on another locale...
  if locID == here.id then return loc;
  else return Locales[locID];
}

inline proc getLocale(arr : [], idx) {
  return getLocale(arr.domain, idx);
}

inline proc createCyclic(dom : domain) {
  return dom dmapped Cyclic(startIdx=dom.low);
}
inline proc createCyclic(rng : range) {
  return createCyclic({rng});
}
inline proc createCyclic(sz : integral, startIdx = 1) {
  return createCyclic(startIdx..#sz);
}
inline proc createBlock(dom : domain) {
  return dom dmapped Block(dom);
}
inline proc createBlock(rng : range) {
  return createBlock({rng});
}
inline proc createBlock(sz : integral, startIdx = 1) {
  return createBlock(startIdx..#sz);
}

iter getLines(file : string) : string {
  var f = open(file, iomode.r).reader();
  var tmp : string;
  while f.readline(tmp) do yield tmp;
}

iter getLines(file : string, chunkSize = 1024, param tag : iterKind) : string where tag == iterKind.standalone {
  var chunk : atomic int;
  coforall loc in Locales do on loc {
    coforall tid in 1..#here.maxTaskPar {
      proc p() { return open(file, iomode.r).reader(); }
      var f = p();
      var currentIdx = 0;
      var readChunks = true;
      while readChunks {
        // Claim a chunk...
        var ix = chunk.fetchAdd(chunkSize);
        // Skip ahead to chunk we claimed...
        var tmp : string;
        for 1..#(ix - currentIdx) do f.readline(tmp);
        // Begin processing our chunk...
        for 1..#chunkSize {
          if f.readline(tmp) {
            yield tmp;
          } else {
            readChunks = false;
            break;
          }
        }
      }
    }
  }
}

// Iterator Utilities

// Determine if any elements are true...
// any([false, false, true, false, false]) == true
// any([false, false, false, false, false]) == false
proc any(it : _iteratorRecord) {
  for b in it do if b == true then return true;
  return false;
}

proc all(it : _iteratorRecord) {
  for b in it do if b == false then return false;
  return true;
}
