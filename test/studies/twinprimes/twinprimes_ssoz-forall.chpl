// Chapel port of Jabari Zakiya's twinprimes_ssoz algorithm
// Transliterated from the Crystal/C++ versions by Brad Chamberlain, 2024-02-01
// Status:
// * not much attention paid to performance thus far
//   (note that writeln()s in Chapel are heavyweight, so may not want to time
// * focused on correctness thus far; opportunities to be more Chapeltastic
//   may abound
// * this version is shared-memory parallel only; other files contain dist-mem
// * see README.md in this directory for a bit more information

config param printFromTasks = false,
             printTasks = true,
             printTimings = true;

config const low  = 1,
             high = 1_000_000;

twinprimes_ssoz();

proc twinprimes_ssoz() {
  use Time, TPUtil;

  const (start_num, end_num) = computeStartEnd(low, high);

  if printTasks then writeln("tasks = ", here.maxTaskPar);
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
