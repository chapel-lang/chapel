module FastIntDivide {

  extern record chpl_libdiv_s32_t { };
  extern record chpl_libdiv_u32_t { };
  extern record chpl_libdiv_s64_t { };
  extern record chpl_libdiv_u64_t { };


  extern proc chpl_libdiv_s32_gen(ref divTok: chpl_libdiv_s32_t,
                                  divisor: int(32));
  extern proc chpl_libdiv_u32_gen(ref divTok: chpl_libdiv_u32_t,
                                  divisor: uint(32));
  extern proc chpl_libdiv_s64_gen(ref divTok: chpl_libdiv_s64_t,
                                  divisor: int(64));
  extern proc chpl_libdiv_u64_gen(ref divTok: chpl_libdiv_u64_t,
                                  divisor: uint(64));


  extern proc chpl_libdiv_s32_do(dividend: int(32),
                                 ref divTok: chpl_libdiv_s32_t): int(32);
  extern proc chpl_libdiv_u32_do(dividend: uint(32),
                                 ref divTok: chpl_libdiv_u32_t): uint(32);
  extern proc chpl_libdiv_s64_do(dividend: int(64),
                                 ref divTok: chpl_libdiv_s64_t): int(64);
  extern proc chpl_libdiv_u64_do(dividend: uint(64),
                                 ref divTok: chpl_libdiv_u64_t): uint(64);


  proc chpl_divTok_t(type idxType) type {
    if CHPL_LIBDIVIDE == "none" {
      if isIntegralType(idxType) then return idxType;
      else compilerError("Unsupported fastIntDiv type");
    } else {
      if idxType == int(8) then return chpl_libdiv_s32_t;
      else if idxType == uint(8) then return chpl_libdiv_u32_t;
      else if idxType == int(16) then return chpl_libdiv_s32_t;
      else if idxType == uint(16) then return chpl_libdiv_u32_t;
      else if idxType == int(32) then return chpl_libdiv_s32_t;
      else if idxType == uint(32) then return chpl_libdiv_u32_t;
      else if idxType == int(64) then return chpl_libdiv_s64_t;
      else if idxType == uint(64) then return chpl_libdiv_u64_t;
      else compilerError("Unsupported fastIntDiv type");
    }
  }


  inline proc chpl_libdiv_gen(ref divTok, divisor: ?t) {
    if CHPL_LIBDIVIDE == "none" {
      divTok = divisor;
    } else {
      if t == int(8) {
        chpl_libdiv_s32_gen(divTok, divisor:int(32));
      } else if t == uint(8) {
        chpl_libdiv_u32_gen(divTok, divisor:uint(32));
      } else if t == int(16) {
        chpl_libdiv_s32_gen(divTok, divisor:int(32));
      } else if t == uint(16) {
        chpl_libdiv_u32_gen(divTok, divisor:uint(32));
      } else if t == int(32) {
        chpl_libdiv_s32_gen(divTok, divisor);
      } else if t == uint(32) {
        chpl_libdiv_u32_gen(divTok, divisor);
      } else if t == int(64) {
        chpl_libdiv_s64_gen(divTok, divisor);
      } else if t == uint(64) {
        chpl_libdiv_u64_gen(divTok, divisor);
      } else {
        compilerError("Unsupported fastIntDiv type");
      }
    }
  }


  inline proc chpl_libdiv_do(dividend: ?t, ref divTok) {
    if CHPL_LIBDIVIDE == "none" {
      return dividend / divTok;
    } else {
      if t == int(8) {
        return chpl_libdiv_s32_do(dividend:int(32), divTok):int(8);
      } else if t == uint(8) {
        return chpl_libdiv_u32_do(dividend:int(32), divTok):int(8);
      } else if t == int(16) {
        return chpl_libdiv_s32_do(dividend:int(32), divTok):int(16);
      } else if t == uint(16) {
        return chpl_libdiv_u32_do(dividend:int(32), divTok):int(16);
      } else if t == int(32) {
        return chpl_libdiv_s32_do(dividend, divTok);
      } else if t == uint(32) {
        return chpl_libdiv_u32_do(dividend, divTok);
      } else if t == int(64) {
        return chpl_libdiv_s64_do(dividend, divTok);
      } else if t == uint(64) {
        return chpl_libdiv_u64_do(dividend, divTok);
      } else {
        compilerError("Unsupported fastIntDiv type");
      }
    }
  }
}
