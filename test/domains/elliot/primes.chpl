// Based on https://github.com/chapel-lang/chapel/issues/15874
// Stress repeated adding/removing from associative types
use Time, Map;

config const perf = true;
config const LIMIT = 10000000;

type Prime = uint(32);

class Primes { // needed so we can use next to get successive values
  type tableType;
  var n: Prime; var obp: Prime; var q: Prime;
  var bps: owned Primes(tableType)?;
  var dict = new tableType();
  proc next(): Prime { // odd primes!
    if this.n < 5 then { this.n = 5; return 3; }
    if this.bps == nil {
      this.bps = new Primes(tableType); // secondary odd base primes feed
      this.obp = this.bps!.next(); this.q = this.obp * this.obp;
    }
    while true {
      if this.n >= this.q then { // advance secondary stream of base primes...
        const adv = this.obp * 2; const key = this.q + adv;
        this.obp = this.bps!.next(); this.q = this.obp * this.obp;
        this.dict.add(key, adv);
      }
      else if this.dict.contains(this.n) then { // found a composite; advance...
        const adv = this.dict[this.n]; this.dict.remove(this.n);
        var nkey = this.n + adv;
        while this.dict.contains(nkey) do nkey += adv;
        this.dict.add(nkey, adv);
      }
      else { const p = this.n; this.n += 2; return p; }
      this.n += 2;
    }
    return 0; // to keep compiler happy in returning a value!
  }
  iter these(): Prime { yield 2; while true do yield this.next(); }
}

proc primePerf(type tableType) {
  var count = 0;
  if !perf {
    write("The first 25 primes are:  ");
    for p in new Primes(tableType) { if count >= 25 then break; write(p, " "); count += 1; }
    writeln();
  } else {
    var timer: Timer;
    timer.start();
    for p in new Primes(tableType) { if p > LIMIT then break; count += 1; }
    timer.stop();
    writef("%16s: %7.1dr s\n", tableType:string, timer.elapsed());
  }
}

proc main() {
  primePerf(AssocArrTable);
  primePerf(MapTable);
  primePerf(BasePrimesTable);
}

record MapTable {
  var dict: map(Prime, Prime, parSafe=false);
  forwarding dict;
}

record AssocArrTable {
  var keys: domain(Prime, parSafe=false); var dict: [keys] Prime;
  inline proc add(key, val) { this.keys += key; this.dict[key] = val; }
  inline proc contains(key) { return this.keys.contains(key); }
  inline proc this(key) { return this.dict[key]; }
  inline proc remove(key) { this.keys.remove(key);}
}

record BasePrimesTable { // specialized for the use here...
  record BasePrimeEntry { var fullkey: Prime; var val: Prime; }
  var cpcty: int = 8; var sz: int = 0;
  var dom = { 0 .. cpcty - 1 }; var bpa: [dom] BasePrimeEntry;
  proc grow() {
    const ndom = dom; var cbpa: [ndom] BasePrimeEntry = bpa[ndom];
    bpa = new BasePrimeEntry(); cpcty *= 2; dom = { 0 .. cpcty - 1 };
    for kv in cbpa do if kv.fullkey != 0 then add(kv.fullkey, kv.val);
  }
  proc find(k: Prime): int { // internal get location of value or -1
    const msk = cpcty - 1; var skey = k: int & msk;
    var perturb = k: int; var loop = 8;
    do {
      if bpa[skey].fullkey == k then return skey;
      perturb >>= 5; skey = (5 * skey + 1 + perturb) & msk;
      loop -= 1; if perturb > 0 then loop = 8;
    } while loop > 0;
    return -1; // not found!
  }
  proc contains(k: Prime): bool { return find(k) >= 0; }
  proc add(k, v: Prime) { // if exists then replaces else new entry
    const fndi = find(k);
    if fndi >= 0 then bpa[fndi] = new BasePrimeEntry(k, v);
    else {
      sz += 1; if 2 * sz > cpcty then grow();
      const msk = cpcty - 1; var skey = k: int & msk;
      var perturb = k: int; var loop = 8;
      do {
        if bpa[skey].fullkey == 0 then {
          bpa[skey] = new BasePrimeEntry(k, v); return; }
        perturb >>= 5; skey = (5 * skey + 1 + perturb) & msk;
        loop -= 1; if perturb > 0 then loop = 8;
      } while loop > 0;
    }
  }
  proc remove(k: Prime) { // if doesn't exist does nothing
    const fndi = find(k);
    if fndi >= 0 then { bpa[fndi].fullkey = 0; sz -= 1; }
  }
  proc this(k: Prime): Prime { // returns value or 0 if not found
    const fndi = find(k);
    if fndi < 0 then return 0; else return bpa[fndi].val;
  }
}
