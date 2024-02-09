proc computeStartEnd(lo, hi) {
  var start_num  = max(lo, 3);
  var end_num = max(hi, 3);

  if start_num > end_num then start_num <=> end_num;

  start_num |= 1;
  end_num = (end_num - 1) | 1;

  if (end_num - start_num) < 2 {
    start_num = 7;
    end_num = 7;
  }
  return (start_num, end_num);
}

proc set_sieve_parameters(start_num, end_num) {
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
