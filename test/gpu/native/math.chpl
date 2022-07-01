use Math;
use GPUDiagnostics;

config const verbose = false;

proc main() {

  var i8: int(8) = 10;
  var i16: int(16) = 10;
  var i32: int(32) = 10;
  var i64: int(64) = 10;
  var u8: uint(8) = 10;
  var u16: uint(16) = 10;
  var u32: uint(32) = 10;
  var u64: uint(64) = 10;

  var im32: imag(32) = 10i;
  var im64: imag(64) = 10i;

  /* TODO
  var c64: complex(64) = 10+10i;
  var c128: complex(128) = 10+10i;
  */

  var r32: real(32) = 0.1;
  var r64: real(64) = 0.1;

  var r32b: real(32) = 10.0;
  var r64b: real(64) = 10.0;

  on here.gpus[0] {
    var R: [0..1] real;
    var I: [0..1] imag;
    var B: [0..1] bool;

    const r = 0..0;

    proc check(A, s) {
      if getGPUDiagnostics()[0].kernel_launch !=1 then
        writeln(s + " didn't result in kernel launch");
      else if verbose then
        writeln(s + " resulted in kernel launch");

      const isCorrect = if A.eltType == bool then A[0]==A[1] else isclose(A[0],A[1]);
      if !isCorrect then
        writeln(s + " computed wrong result. ("+A[0]:string+", "+A[1]:string+")");
      else if verbose then
        writeln(s + " computed right result. ("+A[0]:string+", "+A[1]:string+")");

      resetGPUDiagnostics();
    }

    startGPUDiagnostics();

// foreach loops should result in a kernel launch that sets the 0th index of the array, the
// following statement is outside the foreach, so wll execute on the host. We check whether they
// produce the same result and whether the loop has actually turned into a kernel launch
foreach i in r do  R[0] = abs(i8 );   R[1] = abs(i8 );  check(R,"abs(i8 )");
foreach i in r do  R[0] = abs(i16);   R[1] = abs(i16);  check(R,"abs(i16)");
foreach i in r do  R[0] = abs(i32);   R[1] = abs(i32);  check(R,"abs(i32)");
foreach i in r do  R[0] = abs(i64);   R[1] = abs(i64);  check(R,"abs(i64)");
foreach i in r do  R[0] = abs(u8 );   R[1] = abs(u8 );  check(R,"abs(u8 )");
foreach i in r do  R[0] = abs(u16);   R[1] = abs(u16);  check(R,"abs(u16)");
foreach i in r do  R[0] = abs(u32);   R[1] = abs(u32);  check(R,"abs(u32)");
foreach i in r do  R[0] = abs(u64);   R[1] = abs(u64);  check(R,"abs(u64)");
foreach i in r do  R[0] = abs(r32);   R[1] = abs(r32);  check(R,"abs(r32)");
foreach i in r do  R[0] = abs(r64);   R[1] = abs(r64);  check(R,"abs(r64)");

foreach i in r do  R[0] = abs(im32);   R[1] = abs(im32);  check(R,"abs(im32)");
foreach i in r do  R[0] = abs(im64);   R[1] = abs(im64);  check(R,"abs(im64)");

/* TODO complex support
foreach i in r do  R[0] = abs(c64 );   R[1] = abs(c64 );  check(R,R);
foreach i in r do  R[0] = abs(c128);   R[1] = abs(c128);  check(R,R);
*/

foreach i in r do  R[0] = acos(r32)  ;   R[1] = acos(r32);  check(R,"acos(r32)");
foreach i in r do  R[0] = acos(r64)  ;   R[1] = acos(r64);  check(R,"acos(r64)");
foreach i in r do  R[0] = acosh(r32b);   R[1] = acosh(r32b);  check(R,"acosh(r32b)");
foreach i in r do  R[0] = acosh(r64b);   R[1] = acosh(r64b);  check(R,"acosh(r64b)");

foreach i in r do  R[0] = asin(r32) ;   R[1] = asin(r32);  check(R,"asin(r32)");
foreach i in r do  R[0] = asin(r64) ;   R[1] = asin(r64);  check(R,"asin(r64)");
foreach i in r do  R[0] = asinh(r32);   R[1] = asinh(r32);  check(R,"asinh(r32)");
foreach i in r do  R[0] = asinh(r64);   R[1] = asinh(r64);  check(R,"asinh(r64)");

foreach i in r do  R[0] = atan(r32)     ; R[1] = atan(r32);  check(R,"atan(r32)");
foreach i in r do  R[0] = atan(r64)     ; R[1] = atan(r64);  check(R,"atan(r64)");
foreach i in r do  R[0] = atan2(r32,r32); R[1] = atan2(r32,r32);  check(R,"atan2(r32,r32)");
foreach i in r do  R[0] = atan2(r64,r64); R[1] = atan2(r64,r64);  check(R,"atan2(r64,r64)");
foreach i in r do  R[0] = atanh(r32)    ; R[1] = atanh(r32);  check(R,"atanh(r32)");
foreach i in r do  R[0] = atanh(r64)    ; R[1] = atanh(r64);  check(R,"atanh(r64)");

foreach i in r do  R[0] = cbrt(r32);   R[1] = cbrt(r32);  check(R,"cbrt(r32)");
foreach i in r do  R[0] = cbrt(r64);   R[1] = cbrt(r64);  check(R,"cbrt(r64)");

foreach i in r do  R[0] = ceil(r32);   R[1] = ceil(r32);  check(R,"ceil(r32)");
foreach i in r do  R[0] = ceil(r64);   R[1] = ceil(r64);  check(R,"ceil(r64)");

foreach i in r do  R[0] = conjg(i8  ); R[1] = conjg(i8);  check(R,"conjg(i8)");
foreach i in r do  R[0] = conjg(i16 ); R[1] = conjg(i16);  check(R,"conjg(i16)");
foreach i in r do  R[0] = conjg(i32 ); R[1] = conjg(i32);  check(R,"conjg(i32)");
foreach i in r do  R[0] = conjg(i64 ); R[1] = conjg(i64);  check(R,"conjg(i64)");
foreach i in r do  R[0] = conjg(u8  ); R[1] = conjg(u8);  check(R,"conjg(u8)");
foreach i in r do  R[0] = conjg(u16 ); R[1] = conjg(u16);  check(R,"conjg(u16)");
foreach i in r do  R[0] = conjg(u32 ); R[1] = conjg(u32);  check(R,"conjg(u32)");
foreach i in r do  R[0] = conjg(u64 ); R[1] = conjg(u64);  check(R,"conjg(u64)");
foreach i in r do  I[0] = conjg(im32); I[1] = conjg(im32);  check(I,"conjg(im32)");
foreach i in r do  I[0] = conjg(im64); I[1] = conjg(im64);  check(I,"conjg(im64)");
foreach i in r do  R[0] = conjg(r32 ); R[1] = conjg(r32);  check(R,"conjg(r32)");
foreach i in r do  R[0] = conjg(r64 ); R[1] = conjg(r64);  check(R,"conjg(r64)");

foreach i in r do  R[0] =  cos(r32) ; R[1] =  cos(r32);  check(R,"cos(r32)");
foreach i in r do  R[0] =  cos(r64) ; R[1] =  cos(r64);  check(R,"cos(r64)");
foreach i in r do  R[0] =  cosh(r32); R[1] =  cosh(r32);  check(R,"cosh(r32)");
foreach i in r do  R[0] =  cosh(r64); R[1] =  cosh(r64);  check(R,"cosh(r64)");

// the following are implemented in Chapel, but let's test them here for completeness
foreach i in r do R[0] = divceil(i8 ,i8 );     R[1] = divceil(i8 ,i8 );     check(R,"divceil(i8,i8)");
foreach i in r do R[0] = divceil(i16,i16);     R[1] = divceil(i16,i16);     check(R,"divceil(i16,i16)");
foreach i in r do R[0] = divceil(i32,i32);     R[1] = divceil(i32,i32);     check(R,"divceil(i32,i32)");
foreach i in r do R[0] = divceil(i64,i64);     R[1] = divceil(i64,i64);     check(R,"divceil(i64,i64)");
foreach i in r do R[0] = divceil(u8 ,u8 );     R[1] = divceil(u8 ,u8 );     check(R,"divceil(u8,u8)");
foreach i in r do R[0] = divceil(u16,u16);     R[1] = divceil(u16,u16);     check(R,"divceil(u16,u16)");
foreach i in r do R[0] = divceil(u32,u32);     R[1] = divceil(u32,u32);     check(R,"divceil(u32,u32)");
foreach i in r do R[0] = divceil(u64,u64);     R[1] = divceil(u64,u64);     check(R,"divceil(u64,u64)");
foreach i in r do R[0] = divceilpos(i8 ,i8 );  R[1] = divceilpos(i8 ,i8 );  check(R,"divceilpos(i8,i8)");
foreach i in r do R[0] = divceilpos(i16,i16);  R[1] = divceilpos(i16,i16);  check(R,"divceilpos(i16,i16)");
foreach i in r do R[0] = divceilpos(i32,i32);  R[1] = divceilpos(i32,i32);  check(R,"divceilpos(i32,i32)");
foreach i in r do R[0] = divceilpos(i64,i64);  R[1] = divceilpos(i64,i64);  check(R,"divceilpos(i64,i64)");
foreach i in r do R[0] = divfloor(i8 ,i8 );    R[1] = divfloor(i8 ,i8 );    check(R,"divfloor(i8,i8)");
foreach i in r do R[0] = divfloor(i16,i16);    R[1] = divfloor(i16,i16);    check(R,"divfloor(i16,i16)");
foreach i in r do R[0] = divfloor(i32,i32);    R[1] = divfloor(i32,i32);    check(R,"divfloor(i32,i32)");
foreach i in r do R[0] = divfloor(i64,i64);    R[1] = divfloor(i64,i64);    check(R,"divfloor(i64,i64)");
foreach i in r do R[0] = divfloor(u8 ,u8 );    R[1] = divfloor(u8 ,u8 );    check(R,"divfloor(u8,u8)");
foreach i in r do R[0] = divfloor(u16,u16);    R[1] = divfloor(u16,u16);    check(R,"divfloor(u16,u16)");
foreach i in r do R[0] = divfloor(u32,u32);    R[1] = divfloor(u32,u32);    check(R,"divfloor(u32,u32)");
foreach i in r do R[0] = divfloor(u64,u64);    R[1] = divfloor(u64,u64);    check(R,"divfloor(u64,u64)");
foreach i in r do R[0] = divfloorpos(i8 ,i8 ); R[1] = divfloorpos(i8 ,i8 ); check(R,"divfloorpos(i8,i8)");
foreach i in r do R[0] = divfloorpos(i16,i16); R[1] = divfloorpos(i16,i16); check(R,"divfloorpos(i16,i16)");
foreach i in r do R[0] = divfloorpos(i32,i32); R[1] = divfloorpos(i32,i32); check(R,"divfloorpos(i32,i32)");
foreach i in r do R[0] = divfloorpos(i64,i64); R[1] = divfloorpos(i64,i64); check(R,"divfloorpos(i64,i64)");

foreach i in r do  R[0] =  erf(r32) ;   R[1] =  erf(r32);  check(R,"erf(r32)");
foreach i in r do  R[0] =  erf(r64) ;   R[1] =  erf(r64);  check(R,"erf(r64)");
foreach i in r do  R[0] =  erfc(r32);   R[1] =  erfc(r32);  check(R,"erfc(r32)");
foreach i in r do  R[0] =  erfc(r64);   R[1] =  erfc(r64);  check(R,"erfc(r64)");
foreach i in r do  R[0] =  exp(r32) ;   R[1] =  exp(r32);  check(R,"exp(r32)");
foreach i in r do  R[0] =  exp(r64) ;   R[1] =  exp(r64);  check(R,"exp(r64)");
foreach i in r do  R[0] =  exp2(r32);   R[1] =  exp2(r32);  check(R,"exp2(r32)");
foreach i in r do  R[0] =  exp2(r64);   R[1] =  exp2(r64);  check(R,"exp2(r64)");

foreach i in r do  R[0] =  floor(r32);   R[1] =  floor(r32);  check(R,"floor(r32)");
foreach i in r do  R[0] =  floor(r64);   R[1] =  floor(r64);  check(R,"floor(r64)");

foreach i in r do R[0] = gcd(i8 ,i8 ); R[1] = gcd(i8 ,i8 ); check(R,"gcd(i8,i8)");
foreach i in r do R[0] = gcd(i16,i16); R[1] = gcd(i16,i16); check(R,"gcd(i16,i16)");
foreach i in r do R[0] = gcd(i32,i32); R[1] = gcd(i32,i32); check(R,"gcd(i32,i32)");
foreach i in r do R[0] = gcd(i64,i64); R[1] = gcd(i64,i64); check(R,"gcd(i64,i64)");

foreach i in r do  B[0] =  isfinite(r32);   B[1] =  isfinite(r32);  check(B,"isfinite(r32)");
foreach i in r do  B[0] =  isfinite(r64);   B[1] =  isfinite(r64);  check(B,"isfinite(r64)");

foreach i in r do  B[0] =  isinf(r32);   B[1] =  isinf(r32);  check(B,"isinf(r32)");
foreach i in r do  B[0] =  isinf(r64);   B[1] =  isinf(r64);  check(B,"isinf(r64)");

foreach i in r do  B[0] =  isnan(r32);   B[1] =  isnan(r32);  check(B,"isnan(r32)");
foreach i in r do  B[0] =  isnan(r64);   B[1] =  isnan(r64);  check(B,"isnan(r64)");

foreach i in r do  R[0] = ldexp(r32,i32);   R[1] =  ldexp(r32,i32);  check(R,"ldexp(r32,i32)");
foreach i in r do  R[0] = ldexp(r64,i32);   R[1] =  ldexp(r64,i32);  check(R,"ldexp(r64,i32)");

foreach i in r do  R[0] = lgamma(r32);   R[1] =  lgamma(r32);  check(R,"lgamma(r32)");
foreach i in r do  R[0] = lgamma(r64);   R[1] =  lgamma(r64);  check(R,"lgamma(r64)");

foreach i in r do  R[0] = log(r32);   R[1] =  log(r32);  check(R,"log(r32)");
foreach i in r do  R[0] = log(r64);   R[1] =  log(r64);  check(R,"log(r64)");

foreach i in r do  R[0] = log10(r32);   R[1] =  log10(r32);  check(R,"log10(r32)");
foreach i in r do  R[0] = log10(r64);   R[1] =  log10(r64);  check(R,"log10(r64)");

foreach i in r do  R[0] = log1p(r32);   R[1] =  log1p(r32);  check(R,"log1p(r32)");
foreach i in r do  R[0] = log1p(r64);   R[1] =  log1p(r64);  check(R,"log1p(r64)");

/* TODO runtime support
foreach i in r do  R[0] = logBasePow2(i8 ,2);   R[1] = logBasePow2(i8 ,2);  check(R,"logBasePow2(i8 ,2)");
foreach i in r do  R[0] = logBasePow2(i16,2);   R[1] = logBasePow2(i16,2);  check(R,"logBasePow2(i16,2)");
foreach i in r do  R[0] = logBasePow2(i32,2);   R[1] = logBasePow2(i32,2);  check(R,"logBasePow2(i32,2)");
foreach i in r do  R[0] = logBasePow2(i64,2);   R[1] = logBasePow2(i64,2);  check(R,"logBasePow2(i64,2)");
foreach i in r do  R[0] = logBasePow2(u8 ,2);   R[1] = logBasePow2(u8 ,2);  check(R,"logBasePow2(u8 ,2)");
foreach i in r do  R[0] = logBasePow2(u16,2);   R[1] = logBasePow2(u16,2);  check(R,"logBasePow2(u16,2)");
foreach i in r do  R[0] = logBasePow2(u32,2);   R[1] = logBasePow2(u32,2);  check(R,"logBasePow2(u32,2)");
foreach i in r do  R[0] = logBasePow2(u64,2);   R[1] = logBasePow2(u64,2);  check(R,"logBasePow2(u64,2)");

foreach i in r do  R[0] = log2(i8 );   R[1] = log2(i8 );  check(R,"log2(i8 )");
foreach i in r do  R[0] = log2(i16);   R[1] = log2(i16);  check(R,"log2(i16)");
foreach i in r do  R[0] = log2(i32);   R[1] = log2(i32);  check(R,"log2(i32)");
foreach i in r do  R[0] = log2(i64);   R[1] = log2(i64);  check(R,"log2(i64)");
foreach i in r do  R[0] = log2(u8 );   R[1] = log2(u8 );  check(R,"log2(u8 )");
foreach i in r do  R[0] = log2(u16);   R[1] = log2(u16);  check(R,"log2(u16)");
foreach i in r do  R[0] = log2(u32);   R[1] = log2(u32);  check(R,"log2(u32)");
foreach i in r do  R[0] = log2(u64);   R[1] = log2(u64);  check(R,"log2(u64)");
*/

foreach i in r do  R[0] = max(i8 ,i8 );   R[1] = max(i8 ,i8 );  check(R,"max(i8 ,i8 )");
foreach i in r do  R[0] = max(i16,i16);   R[1] = max(i16,i16);  check(R,"max(i16,i16)");
foreach i in r do  R[0] = max(i32,i32);   R[1] = max(i32,i32);  check(R,"max(i32,i32)");
foreach i in r do  R[0] = max(i64,i64);   R[1] = max(i64,i64);  check(R,"max(i64,i64)");
foreach i in r do  R[0] = max(u8 ,u8 );   R[1] = max(u8 ,u8 );  check(R,"max(u8 ,u8 )");
foreach i in r do  R[0] = max(u16,u16);   R[1] = max(u16,u16);  check(R,"max(u16,u16)");
foreach i in r do  R[0] = max(u32,u32);   R[1] = max(u32,u32);  check(R,"max(u32,u32)");
foreach i in r do  R[0] = max(u64,u64);   R[1] = max(u64,u64);  check(R,"max(u64,u64)");
foreach i in r do  R[0] = max(r32,r32);   R[1] = max(r32,r32);  check(R,"max(r32,r32)");
foreach i in r do  R[0] = max(r64,r64);   R[1] = max(r64,r64);  check(R,"max(r64,r64)");
foreach i in r do  R[0] = min(i8 ,i8 );   R[1] = min(i8 ,i8 );  check(R,"min(i8 ,i8 )");
foreach i in r do  R[0] = min(i16,i16);   R[1] = min(i16,i16);  check(R,"min(i16,i16)");
foreach i in r do  R[0] = min(i32,i32);   R[1] = min(i32,i32);  check(R,"min(i32,i32)");
foreach i in r do  R[0] = min(i64,i64);   R[1] = min(i64,i64);  check(R,"min(i64,i64)");
foreach i in r do  R[0] = min(u8 ,u8 );   R[1] = min(u8 ,u8 );  check(R,"min(u8 ,u8 )");
foreach i in r do  R[0] = min(u16,u16);   R[1] = min(u16,u16);  check(R,"min(u16,u16)");
foreach i in r do  R[0] = min(u32,u32);   R[1] = min(u32,u32);  check(R,"min(u32,u32)");
foreach i in r do  R[0] = min(u64,u64);   R[1] = min(u64,u64);  check(R,"min(u64,u64)");
foreach i in r do  R[0] = min(r32,r32);   R[1] = min(r32,r32);  check(R,"min(r32,r32)");
foreach i in r do  R[0] = min(r64,r64);   R[1] = min(r64,r64);  check(R,"min(r64,r64)");

foreach i in r do  R[0] = mod(i8 ,i8 );   R[1] = mod(i8 ,i8 );  check(R,"mod(i8 ,i8 )");
foreach i in r do  R[0] = mod(i16,i16);   R[1] = mod(i16,i16);  check(R,"mod(i16,i16)");
foreach i in r do  R[0] = mod(i32,i32);   R[1] = mod(i32,i32);  check(R,"mod(i32,i32)");
foreach i in r do  R[0] = mod(i64,i64);   R[1] = mod(i64,i64);  check(R,"mod(i64,i64)");
foreach i in r do  R[0] = mod(u8 ,u8 );   R[1] = mod(u8 ,u8 );  check(R,"mod(u8 ,u8 )");
foreach i in r do  R[0] = mod(u16,u16);   R[1] = mod(u16,u16);  check(R,"mod(u16,u16)");
foreach i in r do  R[0] = mod(u32,u32);   R[1] = mod(u32,u32);  check(R,"mod(u32,u32)");
foreach i in r do  R[0] = mod(u64,u64);   R[1] = mod(u64,u64);  check(R,"mod(u64,u64)");
foreach i in r do  R[0] = mod(r32,r32);   R[1] = mod(r32,r32);  check(R,"mod(r32,r32)");
foreach i in r do  R[0] = mod(r64,r64);   R[1] = mod(r64,r64);  check(R,"mod(r64,r64)");

foreach i in r do  R[0] =  nearbyint(r32);   R[1] =  nearbyint(r32);  check(R,"nearbyint(r32)");
foreach i in r do  R[0] =  nearbyint(r64);   R[1] =  nearbyint(r64);  check(R,"nearbyint(r64)");

foreach i in r do  R[0] =  rint(r32);   R[1] =  rint(r32);  check(R,"rint(r32)");
foreach i in r do  R[0] =  rint(r64);   R[1] =  rint(r64);  check(R,"rint(r64)");

foreach i in r do  R[0] =  round(r32);   R[1] =  round(r32);  check(R,"round(r32)");
foreach i in r do  R[0] =  round(r64);   R[1] =  round(r64);  check(R,"round(r64)");

foreach i in r do  R[0] = sgn(i8 );   R[1] = sgn(i8 );  check(R,"sgn(i8 )");
foreach i in r do  R[0] = sgn(i16);   R[1] = sgn(i16);  check(R,"sgn(i16)");
foreach i in r do  R[0] = sgn(i32);   R[1] = sgn(i32);  check(R,"sgn(i32)");
foreach i in r do  R[0] = sgn(i64);   R[1] = sgn(i64);  check(R,"sgn(i64)");
foreach i in r do  R[0] = sgn(u8 );   R[1] = sgn(u8 );  check(R,"sgn(u8 )");
foreach i in r do  R[0] = sgn(u16);   R[1] = sgn(u16);  check(R,"sgn(u16)");
foreach i in r do  R[0] = sgn(u32);   R[1] = sgn(u32);  check(R,"sgn(u32)");
foreach i in r do  R[0] = sgn(u64);   R[1] = sgn(u64);  check(R,"sgn(u64)");
foreach i in r do  R[0] = sgn(r32);   R[1] = sgn(r32);  check(R,"sgn(r32)");
foreach i in r do  R[0] = sgn(r64);   R[1] = sgn(r64);  check(R,"sgn(r64)");

foreach i in r do  B[0] =  signbit(r32);   B[1] =  signbit(r32);  check(B,"signbit(r32)");
foreach i in r do  B[0] =  signbit(r64);   B[1] =  signbit(r64);  check(B,"signbit(r64)");

foreach i in r do  R[0] =  sin(r32);   R[1] =  sin(r32);  check(R,"sin(r32)");
foreach i in r do  R[0] =  sin(r64);   R[1] =  sin(r64);  check(R,"sin(r64)");
foreach i in r do  R[0] =  sinh(r32);   R[1] =  sinh(r32);  check(R,"sinh(r32)");
foreach i in r do  R[0] =  sinh(r64);   R[1] =  sinh(r64);  check(R,"sinh(r64)");

foreach i in r do  R[0] =  sqrt(r32);   R[1] =  sqrt(r32);  check(R,"sqrt(r32)");
foreach i in r do  R[0] =  sqrt(r64);   R[1] =  sqrt(r64);  check(R,"sqrt(r64)");

foreach i in r do  R[0] =  tan(r32);   R[1] =  tan(r32);  check(R,"tan(r32)");
foreach i in r do  R[0] =  tan(r64);   R[1] =  tan(r64);  check(R,"tan(r64)");
foreach i in r do  R[0] =  tanh(r32);   R[1] =  tanh(r32);  check(R,"tanh(r32)");
foreach i in r do  R[0] =  tanh(r64);   R[1] =  tanh(r64);  check(R,"tanh(r64)");

foreach i in r do  R[0] =  tgamma(r32);   R[1] =  tgamma(r32);  check(R,"tgamma(r32)");
foreach i in r do  R[0] =  tgamma(r64);   R[1] =  tgamma(r64);  check(R,"tgamma(r64)");

foreach i in r do  R[0] =  trunc(r32);   R[1] =  trunc(r32);  check(R,"trunc(r32)");
foreach i in r do  R[0] =  trunc(r64);   R[1] =  trunc(r64);  check(R,"trunc(r64)");

foreach i in r do  R[0] =  j0(r32);   R[1] =  j0(r32);  check(R,"j0(r32)");
foreach i in r do  R[0] =  j0(r64);   R[1] =  j0(r64);  check(R,"j0(r64)");
foreach i in r do  R[0] =  j1(r32);   R[1] =  j1(r32);  check(R,"j1(r32)");
foreach i in r do  R[0] =  j1(r64);   R[1] =  j1(r64);  check(R,"j1(r64)");
foreach i in r do  R[0] =  jn(2,r32);   R[1] =  jn(2,r32);  check(R,"jn(2,r32)");
foreach i in r do  R[0] =  jn(2,r64);   R[1] =  jn(2,r64);  check(R,"jn(2,r64)");

foreach i in r do  R[0] =  y0(r32);   R[1] =  y0(r32);  check(R,"y0(r32)");
foreach i in r do  R[0] =  y0(r64);   R[1] =  y0(r64);  check(R,"y0(r64)");
foreach i in r do  R[0] =  y1(r32);   R[1] =  y1(r32);  check(R,"y1(r32)");
foreach i in r do  R[0] =  y1(r64);   R[1] =  y1(r64);  check(R,"y1(r64)");
foreach i in r do  R[0] =  yn(2,r32);   R[1] =  yn(2,r32);  check(R,"yn(2,r32)");
foreach i in r do  R[0] =  yn(2,r64);   R[1] =  yn(2,r64);  check(R,"yn(2,r64)");

    stopGPUDiagnostics();
  }
}
