// Chapel port of Jabari Zakiya's twinprimes_ssoz algorithm
// Transliterated from the Crystal/C++ versions by Brad Chamberlain, 2024-02-01
// Status:
// * not tested very thoroughly or intelligently yet
// * not much attention paid to performance thus far
//   (note that writeln()s in Chapel are heavyweight, so may not want to time
// * focused on correctness thus far; opportunities to be more Chapeltastic
//   may abound
// * shared memory only thus far
// * see TODO list at bottom of file, and throughout, for other thoughts

config param printFromTasks = false,
             printTimings=true;

config const debug = false;

config var start_num = 3,
           end_num = 3;

twinprimes_ssoz();

proc twinprimes_ssoz() {
  use Time;

  start_num  = max(start_num, 3);
  end_num = max(end_num, 3);

  if start_num > end_num then start_num <=> end_num;

  start_num |= 1;
  end_num = (end_num - 1) | 1;

  if (end_num - start_num) < 2 {
    start_num = 7;
    end_num = 7;
  }

  writeln("tasks = ", here.maxTaskPar);
  var ts: stopwatch;
  ts.start();

  const (modpg, res_0, ks, kmin, kmax, krange, pairscnt, restwins, resinvrs) =
    set_sieve_parameters(start_num, end_num);
  const primes = sozp5(sqrt(end_num):uint, res_0, start_num, end_num);

  writeln("each of ", pairscnt, " threads has nextp[", 2, " x ", primes.size,
          "] array");

  var twinscnt: uint, last_twin = min(uint);
  const lo_range = restwins[0] - 3;
  for tp in [3, 5, 11, 17] {
    if end_num == 3 then break;
    if start_num <= tp && tp < lo_range then twinscnt += 1;
  }
  const te = ts.elapsed();
  if printTimings {
    writeln("setup time = ", te, " secs");
    writeln("perform twinprimes ssoz sieve");
    ts.clear();
  }

  // TODO: Why not just use the loop index 'i' rather than this atomic?
  var threadscnt: atomic uint;

  // NOTE: I used reduce intents here because it seemed more elegant
  // than a results array that needed to be reduced after the fact
  // (which could also result in false sharing)
  //  
  forall i in 0..<pairscnt with (+ reduce twinscnt, max reduce last_twin) {
    const (lt, tc) = twins_sieve(restwins[i], kmin, kmax, ks, start_num,
                                 end_num, modpg, primes, resinvrs);
    last_twin = max(last_twin, lt);
    twinscnt += tc;
    if printFromTasks then
      writeln("\r", threadscnt.fetchAdd(1), " of ", pairscnt,
              " twinpairs done");
  }
  writeln("\r", pairscnt, " of ", pairscnt, " twinpairs done");

  if end_num == 5 && twinscnt == 1 then last_twin = 5;
  var kn = krange % ks;
  if kn == 0 then kn = ks;

  if printTimings {
    const t2 = ts.elapsed();
    writeln("sieve time = ", t2, " secs");
    writeln("total time = ", t2 + te, " secs");
  }
  writeln("last segment = ", kn, " resgroups; segment slices = ",
          (krange-1)/ks + 1);
  writeln("total twins = ", twinscnt, "; last twin = ", (last_twin - 1),
          "+/-1");
}

proc twins_sieve(r_hi, kmin, kmax, ks, start_num, end_num, modpg, primes,
                 resinvrs) {
  use BitOps;

  var s = 6,
      bmask = (1 << s) - 1,
      sum = 0: uint,
      ki: uint = kmin - 1,
      kn = ks,
      hi_tp: uint,
      k_max: uint = kmax,
      seg: [0..<((ks-1) >> s) + 1] uint;
  if r_hi - 2 < (start_num - 2) % modpg + 2 then ki += 1;
  if r_hi > (end_num - 2) % modpg + 2 then k_max -= 1;
  var nextp = nextp_init(r_hi, ki, modpg, primes, resinvrs);
  while ki < k_max {
    if ks > (k_max - ki) then kn = k_max - ki;
    // TODO: Could this be a foreach?
    for j in primes.indices {
      const prime = primes[j];
      var k1 = nextp[j << 1];
      while k1 < kn {
        seg[k1 >> s] |= 1 << (k1 & bmask);
        k1 += prime;
      }
      nextp[j << 1] = k1 - kn;
      var k2 = nextp[j << 1 | 1];
      while k2 < kn {
        seg[k2 >> s] |= 1 << (k2 & bmask);
        k2 += prime;
      }
      nextp[j << 1 | 1] = k2 - kn;
    }

    seg[(kn-1) >> s] |= ~1 << ((kn-1) & bmask);
    // TODO: Is this equivalent to these:
    // const cnt = + reduce popCount(~seg);
    // const cnt = + reduce for s in seg do popCount(~s);
    // const cnt = + reduce for k in 0..(kn-1) >> s do popCount(~seg[k])
    var cnt: uint;
    for k in 0..(kn-1) >> s do cnt += popCount(~seg[k]);
    if cnt > 0 {
      sum += cnt;
      var upk = kn - 1;
      while (seg[upk >> s] & 1 << (upk & bmask)) != 0 do upk -= 1;
      hi_tp = ki + upk;
    }
    ki += ks;

    if ki < k_max then foreach b in 0..((kn-1) >> s) do seg[b] = 0;
  }
  hi_tp = if (r_hi > end_num) || (sum == 0) then 1 else hi_tp * modpg + r_hi;
  return (hi_tp, sum);
}

proc nextp_init(rhi, kmin, modpg, primes, resinvrs) {
  var nextp: [0..<primes.size*2] uint;
  const r_hi = rhi,
        r_lo = rhi - 2;
  // TODO: seems like this could be a foreach?
  for j in 0..<primes.size {
    const prime = primes[j],
          k = (prime - 2) / modpg,
          r = (prime - 2) % modpg + 2,
          r_inv = resinvrs[r: int]: uint;
    var rl = (r_inv * r_lo - 2) % modpg + 2,
        rh = (r_inv * r_hi - 2) % modpg + 2,
        kl = k * (prime + rl) + (r * rl - 2) / modpg,
        kh = k * (prime + rh) + (r * rh - 2) / modpg;
    if kl < kmin { kl = (kmin - kl) % prime; if kl > 0 then kl = prime - kl; }
            else { kl -= kmin; }
    if kh < kmin { kh = (kmin - kh) % prime; if kh > 0 then kh = prime - kh; }
            else { kh -= kmin; }
    nextp[j << 1] = kl;
    nextp[j << 1 | 1] = kh;
  }
  return nextp;
}

proc set_sieve_parameters(start_num, end_num) {
  if debug then writeln((start_num, end_num));
  const nrange = end_num - start_num;
  var bn = 0, pg = 3;
  if end_num < 49 {
    bn = 1; pg = 3;
  } else if nrange < 77_000_000 {
    bn = 16; pg = 5;
  } else if nrange < 1_100_000_000 {
    bn = 32; pg = 7;
  } else if nrange < 35_500_000_000 {
    bn = 64; pg = 11;
  } else if nrange < 14_000_000_000_000 {
    pg = 13;
    if nrange > 7_000_000_000_000 then bn = 384;
    else if nrange > 2_500_000_000_000 then bn = 320;
    else if nrange > 250_000_000_000 then bn = 196;
    else bn = 128;
  } else {
    bn = 384; pg = 17;
  }
  const (modpg, res_0, pairscnt, restwins, resinvrs) = gen_pg_parameters(pg);
  const kmin = (start_num-2) / modpg + 1,
        kmax = (end_num - 2) / modpg + 1,
        krange = kmax - kmin + 1,
        n = if krange < 37_500_000_000_000 then 4 else
            if krange < 975_000_000_000_000 then 6
            else 8,
        b = bn * 1024 * n,
        ks: uint = min(krange, b);

  writeln("segment size = ", ks, " resgroups; seg array is [1 x ",
          ((ks-1) >> 6) + 1, "] 64-bits");
  const maxpairs = krange * pairscnt;
  writeln("twinprime candidates = ", maxpairs, "; resgroups = ", krange);
  return (modpg, res_0, ks, kmin, kmax, krange, pairscnt, restwins, resinvrs);
}

proc gen_pg_parameters(prime) {
  use Sort;

  writeln("using Prime Generator parameters for P", prime);
  const primes = [2, 3, 5, 7, 11, 13, 17, 19, 23];
  var modpg = 1, res_0 = 0;
  for prm in primes {
    res_0 = prm;
    if prm > prime then break;
    modpg *= prm;
  }
  var inverses: [0..<modpg + 2] int;
  var restwins = gen_restwins(modpg, inverses);
  sort(restwins);
  if debug then writeln(restwins);
  inverses[modpg + 1] = 1;
  inverses[modpg - 1] = modpg - 1;
  return (modpg, res_0, restwins.size, restwins, inverses);
}

iter gen_restwins(modpg, inverses) {
  var rc = 5, inc = 2, res = 0;
  while rc < (modpg >> 1) {
    if coprime(rc, modpg) {
      const mc = modpg - rc;
      inverses[rc] = modinv(rc, modpg);
      inverses[mc] = modinv(mc, modpg);
      if res + 2 == rc {
        yield rc;
        yield mc + 2;
      }
      res = rc;
    }
    rc += inc;
    inc ^= 0b110;
  }
  yield modpg + 1;
}

proc coprime(in m, in n) {
  while m|1 != 1 {
    const t = m;
    m = n % m;
    n = t;
  }
  return m > 0;
}

proc modinv(a0, m0) {
  if m0 == 1 then return 1;
  var a = a0, m = m0,
      x0 = 0, inv = 1;
  while a > 1 {
    inv -= (a / m) * x0;
    (a, m) = (m, a % m);
    x0 <=> inv;
  }
  if inv < 0 then inv += m0;
  return inv;
}

iter sozp5(val, res_0, start_num, end_num) {
  if end_num < 49 {
    yield 5;
    return;
  }
  const md = 30: uint,
        rescnt = 8,
        res = [7,11,13,17,19,23,29,31],
        bitn = [0,0,0,0,0,1,0,0,0,2,0,4,0,0,0,8,0,16,0,0,0,32,0,0,0,0,0,64,0,
                128]: uint(8),
        range_size = end_num - start_num,  // uint
        kmax: uint = (val - 2) / md + 1;
  var prms: [0..<kmax] uint(8);
  const sqrtn = sqrt(val):int,
        k = sqrtn/md,
        resk = sqrtn - md*k;
  var r = 0; 
  while resk >= res[r] do r += 1;
  const pcs_to_sqrtn = k*rescnt + r;
  // TODO: This conditional is unfortunate...  rethink behavior of 0..<0??
  if pcs_to_sqrtn != 0 then for i in 0:uint..<pcs_to_sqrtn {
    const k = (i/rescnt): uint,
          r = (i%rescnt): int;
    if prms[k] & (1 << r) != 0 then continue;
    const prm_r: uint = res[r],
          prime = md*k + prm_r,
          rem = start_num % prime;
    if !(prime - rem <= range_size || rem == 0) then continue;
    for ri in res {
      const prod: uint = prm_r * ri - 2,
            bit_r = bitn[(prod % md): int];
      var kpm: uint = k * (prime + ri) + prod / md;
      while kpm < kmax {
        prms[kpm] |= bit_r;
        kpm += prime;
      }
    }
  }
  for r in 0..<rescnt {
    for k in 0..<kmax:uint {
      if (prms[k] & (1 << r)) == 0 {
        const prime: uint(64) = md * k + res[r],
              rem = start_num % prime;
        if (res_0 <= prime && prime <= val) &&
           (prime - rem <= range_size || rem == 0) then yield prime;
      }
    }
  }
}


// TODO: 32 vs. 64-bit?
// TODO: if ( ... etc.
// TODO: remove debugging
// TODO: use ranges instead of start/end pairs of integers??
// TODO: name some of these common ranges / promote to domains?
// TODO: try nested iterators to avoid long argument lists?
// TODO: what would it take to make this distributed?  Could create a block-
//       distributed index set for the 0..<pairscnt space for starters...
//       what data would want/need to be localized?
