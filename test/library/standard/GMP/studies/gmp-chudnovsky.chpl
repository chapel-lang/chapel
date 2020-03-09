/* Pi computation using Chudnovsky's algortithm.

 * Copyright 2002, 2005 Hanhong Xue (macroxue at yahoo dot com)

 * Slightly modified 2005 by Torbjorn Granlund (tege at swox dot com) to allow
   more than 2G digits to be computed.

 * Translated into Chapel by Brad Chamberlain, 2009-2010

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

use GMP, Time, SysCTypes;

param A = 13591409,
      B = 545140134,
      C = 640320,
      D = 12;

param BITS_PER_DIGIT  = 3.32192809488736234787,
      DIGITS_PER_ITER = 14.1816474627254776555,
      DOUBLE_PREC     = 53;

config param HAVE_DIVEXACT_PREINV = false;

config param printTimings = true;

proc CHECK_MEMUSAGE {
  //  writeln("CHECK_MEM_USAGE not implemented yet");  
}

////////////////////////////////////////////////////////////////////////////


// TODO:
// Can variables be moved in to tighter scopes?
// Can casts between ulong and long be avoided/minimized?
// Can writeln be used to wrap gmp_printf and the like?
// Would we want to overload operators on these types?
// Refactor timing code into a routine
// Implement memory checks
// Fix timings -- currently they're bogus in some cases
// Use classes rather than records for fac_t and/or sieve_t?

var t1, t2: mpf_t;

config param do_gcd_time = false;

config const   d: c_long = 100,
             _out: c_int = 1;

const terms   = (d/DIGITS_PER_ITER): c_long,
      percent = terms/100.0;


record fac_t {
  var num_facs: c_long;
  var sdom: domain(1);
  var fac, pow: [sdom] c_ulong;
};

record sieve_t {
  var fac, pow, nxt: c_long;
};

const sieve_size = max(3*5*23*29+1, terms*6): c_long;
var sievedom = {0: c_long..#(sieve_size/2)};
var sieve: [sievedom] sieve_t;

var ftmp, fmul: fac_t;

param INIT_FACS = 32;

var gcd_time: c_long;

var gcd, mgcd: mpz_t;

var depth: c_long = 1;
while ((1<<depth) < terms) do
  depth += 1;
depth += 1;

var stackdom1 = {0..#depth},
    stackdom2 = stackdom1;

var gstack: [stackdom1] mpz_t,
    fpstack, fgstack: [stackdom1] fac_t,
    pstack, qstack: [stackdom2] mpz_t;


var top: c_long = 0;


proc main() {
  writeln("#terms=", terms, ", depth=", depth);

  const start = getCurrentTime();
  write("sieve   ");

  build_sieve(sieve);

  const mid0 = getCurrentTime();
  if (printTimings) then
    writeln("time = ", mid0-start);
  else
    writeln();

  mpz_init(pstack);
  mpz_init(qstack);
  mpz_init(gstack);
  fac_init(fpstack);
  fac_init(fgstack);
  mpz_init(gcd);

  if HAVE_DIVEXACT_PREINV then
    mpz_init(mgcd);

  fac_init(ftmp);
  fac_init(fmul);

  /* begin binary splitting process */
  if (terms <= 0) {
    mpz_set_ui(p2,1);
    mpz_set_ui(q2,0);
    mpz_set_ui(g2,1);
  } else {
    bs(0, terms: c_ulong, 0, 0);
  }

  const mid1 = getCurrentTime();
  write("\nbs      ");
  if (printTimings) then
    writeln("time = ", mid1-mid0);
  else
    writeln();
  write("   gcd  ");
  if (printTimings) then
    writeln("time = ", gcd_time);
  else
    writeln();

  //writeln("misc    ");

  /* free some resources */
  sievedom.clear();

  if HAVE_DIVEXACT_PREINV then
    mpz_clear(mgcd);

  mpz_clear(gcd);
  fac_clear(ftmp);
  fac_clear(fmul);

  for i in 1..depth-1 {
    mpz_clear(pstack[i]);
    mpz_clear(qstack[i]);
    mpz_clear(gstack[i]);
    fac_clear(fpstack[i]);
    fac_clear(fgstack[i]);
  }

  mpz_clear(gstack[0]);
  fac_clear(fpstack[0]);
  fac_clear(fgstack[0]);

  stackdom1.clear();

  /* prepare to convert integers to floats */
  mpf_set_default_prec((d*BITS_PER_DIGIT+16): mp_bitcnt_t);

  /*
	  p*(C/D)*sqrt(C)
    pi = -----------------
	     (q+A*p)
  */

  const psize = mpz_sizeinbase(p1,10),
        qsize = mpz_sizeinbase(q1,10);

  mpz_addmul_ui(q1, p1, A);
  mpz_mul_ui(p1, p1, C/D);

  var pi, qi: mpf_t;

  mpf_init(pi);
  mpf_set_z(pi, p1);
  mpz_clear(p1);

  mpf_init(qi);
  mpf_set_z(qi, q1);
  mpz_clear(q1);

  stackdom2.clear();

  const mid2 = getCurrentTime();

  /* initialize temp float variables for sqrt & div */
  mpf_init(t1);
  mpf_init(t2);
  //mpf_set_prec_raw(t1, mpf_get_prec(pi));

  /* final step */
  write("div     ");
  my_div(qi, pi, qi);
  const mid3 = getCurrentTime();
  if (printTimings) then
    writeln("time = ", mid3-mid2);
  else
    writeln();

  write("sqrt    ");
  my_sqrt_ui(pi, C);
  const mid4 = getCurrentTime();
  if (printTimings) then
    writeln("time = ", mid4-mid3);
  else
    writeln();

  write("mul     ");
  mpf_mul(qi, qi, pi);
  const end = getCurrentTime();
  if (printTimings) then
    writeln("time = ", end - mid4);
  else
    writeln();

  if (printTimings) then
    writeln("total   time = ", end - start);

  writef("   P size=%n digits (%dr)\n", psize, psize:real/d);
  writef("   Q size=%n digits (%dr)\n", qsize, qsize:real/d);
         


  /* output Pi and timing statistics */
  if (_out&1)  {
    writeln("pi(0,", terms, ")=");
    mpf_out_str(chpl_cstdout(), 10:c_int, (d+2):size_t, qi);
    writeln();
  }

  /* free float resources */
  mpf_clear(pi);
  mpf_clear(qi);

  mpf_clear(t1);
  mpf_clear(t2);
}


proc build_sieve(s: [] sieve_t) {
  const n = s.numElements*2,
        m = sqrt(n): c_long;

  s[1/2].fac = 1;
  s[1/2].pow = 1;

  for i in 3..n by 2 {
    if (s[i/2].fac == 0) {
      s[i/2].fac = i;
      s[i/2].pow = 1;
      if (i<=m) {
        for (j,k) in zip(i*i..n by i+i, i/2..) {
	  if (s[j/2].fac==0) {
	    s[j/2].fac = i;
	    if (s[k].fac == i) {
	      s[j/2].pow = s[k].pow + 1;
	      s[j/2].nxt = s[k].nxt;
	    } else {
	      s[j/2].pow = 1;
	      s[j/2].nxt = k;
	    }
	  }
	}
      }
    }
  }
}


proc my_sqrt_ui(ref r: mpf_t, x: c_ulong) {
  const prec0 = mpf_get_prec(r);

  if (prec0 <= DOUBLE_PREC) {
    mpf_set_d(r, sqrt(x));
    return;
  }

  var bits: c_ulong = 0,
      prec = prec0;
  while (prec > DOUBLE_PREC) {
    const bit = prec&1;
    prec = (prec+bit)/2;
    bits = bits*2+bit;
  }

  mpf_set_prec_raw(t1, DOUBLE_PREC);
  mpf_set_d(t1, 1/sqrt(x));

  while (prec < prec0) {
    prec *= 2;
    if (prec < prec0) {
      /* t1 = t1+t1*(1-x*t1*t1)/2; */
      mpf_set_prec_raw(t2, prec);
      mpf_mul(t2, t1, t1);         // half x half -> full
      mpf_mul_ui(t2, t2, x);
      mpf_ui_sub(t2, 1, t2);
      mpf_set_prec_raw(t2, prec/2);
      mpf_div_2exp(t2, t2, 1);
      mpf_mul(t2, t2, t1);         // half x half -> half
      mpf_set_prec_raw(t1, prec);
      mpf_add(t1, t1, t2);
    } else {
      prec = prec0;
      /* t2=x*t1, t1 = t2+t1*(x-t2*t2)/2; */
      mpf_set_prec_raw(t2, prec/2);
      mpf_mul_ui(t2, t1, x);
      mpf_mul(r, t2, t2);          // half x half -> full
      mpf_ui_sub(r, x, r);
      mpf_mul(t1, t1, r);          // half x half -> half
      mpf_div_2exp(t1, t1, 1);
      mpf_add(r, t1, t2);
      break;
    }
    prec -= (bits&1);
    bits /=2;
  }
}


// r = y/x   WARNING: r cannot be the same as y.
proc my_div(ref r: mpf_t, ref y: mpf_t, in x: mpf_t) {
  const prec0 = mpf_get_prec(r);

  if (prec0<=DOUBLE_PREC) {
    mpf_set_d(r, mpf_get_d(y)/mpf_get_d(x));
    return;
  }

  var bits: c_ulong = 0,
      prec = prec0;
  while (prec > DOUBLE_PREC) {
    const bit = prec&1;
    prec = (prec+bit)/2;
    bits = bits*2 + bit;
  }

  mpf_set_prec_raw(t1, DOUBLE_PREC);
  mpf_ui_div(t1, 1, x);

  while (prec < prec0) {
    prec *= 2;
    if (prec < prec0) {
      /* t1 = t1+t1*(1-x*t1); */
      mpf_set_prec_raw(t2, prec);
      mpf_mul(t2, x, t1);          // full x half -> full
      mpf_ui_sub(t2, 1, t2);
      mpf_set_prec_raw(t2, prec/2);
      mpf_mul(t2, t2, t1);         // half x half -> half
      mpf_set_prec_raw(t1, prec);
      mpf_add(t1, t1, t2);
    } else {
      prec = prec0;
      /* t2=y*t1, t1 = t2+t1*(y-x*t2); */
      mpf_set_prec_raw(t2, prec/2);
      mpf_mul(t2, t1, y);          // half x half -> half
      mpf_mul(r, x, t2);           // full x half -> full
      mpf_sub(r, y, r);
      mpf_mul(t1, t1, r);          // half x half -> half
      mpf_add(r, t1, t2);
      break;
    }
    prec -= (bits&1);
    bits /=2;
  }
}

proc fac_show(f: fac_t) {
  for i in 0..#f.num_facs do
    if (f.pow[i]==1) then
      write(f.fac[i], " ");
    else
      write(f.fac[i], "^", f.pow[i], " ");
  writeln();
}

proc fac_reset(ref f: fac_t) {
  f.num_facs = 0;
}

proc fac_init_size(ref f: fac_t, in s: c_long) {
  if (s<INIT_FACS) then
    s=INIT_FACS;

  f.sdom = {0..#s};

  fac_reset(f);
}

proc fac_init(ref f: fac_t) {
  fac_init_size(f, INIT_FACS);
}


proc fac_clear(ref f: fac_t) {
  f.sdom.clear();
}

proc fac_resize(ref f: fac_t, s: c_long)
{
  if (f.sdom.numIndices < s) {
    fac_clear(f);
    fac_init_size(f, s);
  }
}

// BLC: unfortunate -- original GMP took advantage of long/ulong conversions
proc fac_set_bp(ref f: fac_t, base: c_ulong, pow: c_long) {
  assert (base < max(c_long): c_ulong);
  fac_set_bp(f, base: c_long, pow: c_long);
}

// f = base^pow
proc fac_set_bp(ref f: fac_t, in base: c_long, pow: c_long) {
  assert(base < sieve_size);
  var i: c_long;
  base /= 2;
  while (base > 0) {
    f.fac[i] = (sieve[base].fac): c_ulong;
    f.pow[i] = (sieve[base].pow*pow): c_ulong;
    base = sieve[base].nxt;
    i += 1;
  }
  f.num_facs = i;
  assert(i<=f.sdom.numIndices);
}

// r = f*g
proc fac_mul2(ref r: fac_t, f: fac_t, g: fac_t) {

  var i, j, k: c_long;
  while (i<f.num_facs && j<g.num_facs) {
    if (f.fac[i] == g.fac[j]) {
      r.fac[k] = f.fac[i];
      r.pow[k] = f.pow[i] + g.pow[j];
      i += 1; j += 1;
    } else if (f.fac[i] < g.fac[j]) {
      r.fac[k] = f.fac[i];
      r.pow[k] = f.pow[i];
      i += 1;
    } else {
      r.fac[k] = g.fac[j];
      r.pow[k] = g.pow[j];
      j += 1;
    }
    k += 1;
  }
  while (i < f.num_facs) {
    r.fac[k] = f.fac[i];
    r.pow[k] = f.pow[i];
    i += 1;
    k += 1;
  }
  while (j < g.num_facs) {
    r.fac[k] = g.fac[j];
    r.pow[k] = g.pow[j];
    j += 1;
    k += 1;
  }
  r.num_facs = k;
  assert(k<=r.sdom.numIndices);
}

// f *= g
proc fac_mul(ref f: fac_t, g: fac_t) {
  fac_resize(fmul, f.num_facs + g.num_facs);
  fac_mul2(fmul, f, g);
  f <=> fmul;
}

// BLC: unfortunate -- original GMP took advantage of long/ulong conversions
proc fac_mul_bp(ref f: fac_t, base: c_ulong, pow: c_long) {
  assert (base < max(c_long): c_ulong);
  fac_mul_bp(f, base:c_long, pow);
}

// f *= base^pow
proc fac_mul_bp(ref f: fac_t, base: c_long, pow: c_long) {
  fac_set_bp(ftmp, base, pow);
  fac_mul(f, ftmp);
}

// remove factors of power 0
proc fac_compact(ref f: fac_t) {
  var j: c_long;
  for i in 0..#f.num_facs {
    if (f.pow[i]>0) {
      if (j<i) {
	f.fac[j] = f.fac[i];
	f.pow[j] = f.pow[i];
      }
      j += 1;
    }
  }
  f.num_facs = j;
}

// convert factorized form to number
proc bs_mul(ref r: mpz_t, a: c_long, b: c_long) {
  if (b-a <= 32) {
    mpz_set_ui(r, 1);
    for i in a..b-1 do
      for j in 0..fmul.pow[i]-1 do  // BLC: would like to use # here
	mpz_mul_ui(r, r, fmul.fac[i]);
  } else {
    var r2: mpz_t;
    mpz_init(r2);
    bs_mul(r2, a, (a+b)/2);
    bs_mul(r, (a+b)/2, b);
    mpz_mul(r, r, r2);
    mpz_clear(r2);
  }
}


//#if HAVE_DIVEXACT_PREINV
//void mpz_invert_mod_2exp (mpz_ptr, mpz_srcptr);
//void mpz_divexact_pre (mpz_ptr, mpz_srcptr, mpz_srcptr, mpz_srcptr);
//
//#endif

// f /= gcd(f,g), g /= gcd(f,g)
proc fac_remove_gcd(ref p: mpz_t, ref fp: fac_t, ref g: mpz_t, ref fg: fac_t) {
  var i, j, k: c_long;
  var c: c_ulong;
  fac_resize(fmul, min(fp.num_facs, fg.num_facs));
  while (i < fp.num_facs && j < fg.num_facs) {
    if (fp.fac[i] == fg.fac[j]) {
      c = min(fp.pow[i], fg.pow[j]);
      fp.pow[i] -= c;
      fg.pow[j] -= c;
      fmul.fac[k] = fp.fac[i];
      fmul.pow[k] = c;
      i += 1; j += 1; k += 1;
    } else if (fp.fac[i] < fg.fac[j]) {
      i += 1;
    } else {
      j += 1;
    }
  }
  fmul.num_facs = k;
  assert(k <= fmul.sdom.numIndices);

  if (fmul.num_facs) {
    bs_mul(gcd, 0, fmul.num_facs);
    if HAVE_DIVEXACT_PREINV {
      mpz_invert_mod_2exp (mgcd, gcd);
      mpz_divexact_pre (p, p, gcd, mgcd);
      mpz_divexact_pre (g, g, gcd, mgcd);
    } else {
      mpz_tdiv_q(p, p, gcd);
      mpz_tdiv_q(g, g, gcd);
    }
    fac_compact(fp);
    fac_compact(fg);
  }
}


proc p1  ref return  pstack[top];
proc q1  ref return  qstack[top];
proc g1  ref return  gstack[top];
proc fp1 ref return fpstack[top];
proc fg1 ref return fgstack[top];

proc p2  ref return  pstack[top+1];
proc q2  ref return  qstack[top+1];
proc g2  ref return  gstack[top+1];
proc fp2 ref return fpstack[top+1];
proc fg2 ref return fgstack[top+1];

//////////////////////////////////////////////////////////////////////
//////

var progress = 0.0;

/*
proc mpz_t.writeThis(f) throws {
  var outfile = f: file;
    gmp_fprintf(outfile._fp, "%Zd", this);
  if (outfile) then
}
*/

// binary splitting
proc bs(a: c_ulong, b: c_ulong, gflag: c_uint, level: c_long) {
  if (b-a==1) {
    /*
      g(b-1,b) = (6b-5)(2b-1)(6b-1)
      p(b-1,b) = b^3 * C^3 / 24
      q(b-1,b) = (-1)^b*g(b-1,b)*(A+Bb).
    */
    mpz_set_ui(p1, b);
    mpz_mul_ui(p1, p1, b);
    mpz_mul_ui(p1, p1, b);
    mpz_mul_ui(p1, p1, (C/24)*(C/24));
    mpz_mul_ui(p1, p1, C*24);

    mpz_set_ui(g1, 2*b-1);
    mpz_mul_ui(g1, g1, 6*b-1);
    mpz_mul_ui(g1, g1, 6*b-5);

    mpz_set_ui(q1, b);
    mpz_mul_ui(q1, q1, B);
    mpz_add_ui(q1, q1, A);
    mpz_mul   (q1, q1, g1);
    if (b%2) then
      mpz_neg(q1, q1);

    var i = b;
    while ((i&1)==0) do i>>=1;
    fac_set_bp(fp1, i, 3);    // b^3
    fac_mul_bp(fp1, 3*5*23*29, 3);
    fp1.pow[0]-=1;

    fac_set_bp(fg1, 2*b-1, 1);   // 2b-1
    fac_mul_bp(fg1, 6*b-1, 1);   // 6b-1
    fac_mul_bp(fg1, 6*b-5, 1);   // 6b-5

    if (b > (progress:c_ulong)) {
      write(".");
      progress += percent*2;
    }

  } else {
    /*
      p(a,b) = p(a,m) * p(m,b)
      g(a,b) = g(a,m) * g(m,b)
      q(a,b) = q(a,m) * p(m,b) + q(m,b) * g(a,m)
    */
    const mid = (a+(b-a)*0.5224): c_ulong;     // tuning parameter
    bs(a, mid, 1, level+1);

    top += 1;
    bs(mid, b, gflag, level+1);
    top -= 1;

    if (level == 0) then
      write("");

    const ccc = level == 0;

    if (ccc) then CHECK_MEMUSAGE;

    if (level>=4) {           // tuning parameter
      if do_gcd_time {
        const t = getCurrentTime();
        fac_remove_gcd(p2, fp2, g1, fg1);
        gcd_time += getCurrentTime() - t;
      } else {
        fac_remove_gcd(p2, fp2, g1, fg1);
      }
    }

    if (ccc) then CHECK_MEMUSAGE;
    mpz_mul(p1, p1, p2);

    if (ccc) then CHECK_MEMUSAGE;
    mpz_mul(q1, q1, p2);

    if (ccc) then CHECK_MEMUSAGE;
    mpz_mul(q2, q2, g1);

    if (ccc) then CHECK_MEMUSAGE;
    mpz_add(q1, q1, q2);


    if (ccc) then CHECK_MEMUSAGE;
    fac_mul(fp1, fp2);

    if (gflag) {
      mpz_mul(g1, g1, g2);
      fac_mul(fg1, fg2);
    }
  }

  if (_out&2) {
    write("p(", a, ",", b, ")=");
    fac_show(fp1);
    if (gflag) then
      write("g(", a, ",", b, ")=");
    fac_show(fg1);
  }
}


