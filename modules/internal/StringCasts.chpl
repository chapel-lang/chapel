module StringCasts {
  // TODO: I want to break all of these casts from string to T out into
  // T.parse(string), but we dont support methods on types yet. Ideally they
  // would use a tagged union return val as well.

  //
  // Bool
  //
  const _true_s: string = "true";
  const _false_s: string = "false";

  inline proc _cast(type t, x: bool) where t == string {
    if (x) {
      return _true_s;
    } else {
      return _false_s;
    }
  }

  proc _cast(type t, x: string) where t == bool {
    if (x == _true_s) {
      return true;
    } else if (x == _false_s) {
      return false;
    } else {
      halt("Unexpected value when converting from string to bool: '"+x+"'");
    }
  }

  //
  // int
  //
  proc _cast(type t, x: integral) where t == string {
    //TODO: switch to using qio's writef somehow
    extern proc integral_to_c_string_copy(x:int(64), size:uint(32), isSigned: bool) : c_string_copy;
    extern proc strlen(const str: c_string_copy) : size_t;

    var csc = integral_to_c_string_copy(x:int(64), numBytes(x.type), isIntType(x.type));

    var ret: string;
    ret.buff = csc:c_ptr(uint(8));
    ret.len = strlen(csc).safeCast(int);
    ret._size = ret.len+1;

    return ret;
  }

  inline proc _cast(type t, x: string) where isIntegralType(t) {
    //TODO: switch to using qio's readf somehow
    pragma "insert line file info"
    extern proc c_string_to_int8_t  (x:c_string) : int(8);
    pragma "insert line file info"
    extern proc c_string_to_int16_t (x:c_string) : int(16);
    pragma "insert line file info"
    extern proc c_string_to_int32_t (x:c_string) : int(32);
    pragma "insert line file info"
    extern proc c_string_to_int64_t (x:c_string) : int(64);
    pragma "insert line file info"
    extern proc c_string_to_uint8_t (x:c_string) : uint(8);
    pragma "insert line file info"
    extern proc c_string_to_uint16_t(x:c_string) : uint(16);
    pragma "insert line file info"
    extern proc c_string_to_uint32_t(x:c_string) : uint(32);
    pragma "insert line file info"
    extern proc c_string_to_uint64_t(x:c_string) : uint(64);

    if isIntType(t) {
      select numBits(t) {
        when 8  do return c_string_to_int8_t(x.c_str());
        when 16 do return c_string_to_int16_t(x.c_str());
        when 32 do return c_string_to_int32_t(x.c_str());
        when 64 do return c_string_to_int64_t(x.c_str());
        otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
      }
    } else {
      select numBits(t) {
        when 8  do return c_string_to_uint8_t(x.c_str());
        when 16 do return c_string_to_uint16_t(x.c_str());
        when 32 do return c_string_to_uint32_t(x.c_str());
        when 64 do return c_string_to_uint64_t(x.c_str());
        otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
      }
    }
  }

  //
  // real & imag
  //
  inline proc _real_cast_helper(x: real(64), param isImag: bool) : string {
    extern proc real_to_c_string_copy(x:real(64), isImag: bool) : c_string_copy;
    extern proc strlen(const str: c_string_copy) : size_t;

    var csc = real_to_c_string_copy(x:real(64), isImag);

    var ret: string;
    ret.buff = csc:c_ptr(uint(8));
    ret.len = strlen(csc).safeCast(int);
    ret._size = ret.len+1;

    return ret;
  }

  proc _cast(type t, x:real(?w)) where t == string {
    //TODO: switch to using qio's writef somehow
    return _real_cast_helper(x:real(64), false);
  }

  proc _cast(type t, x:imag(?w)) where t == string {
    //TODO: switch to using qio's writef somehow
    // The Chapel version of the imag --> real cast smashes it flat rather than
    // just stripping off the "i".  See the cast in ChapelBase.
    var r = __primitive("cast", real(64), x);
    return _real_cast_helper(r, true);
  }

  inline proc _cast(type t, x: string) where isRealType(t) {
    pragma "insert line file info"
    extern proc c_string_to_real32(x: c_string) : real(32);
    pragma "insert line file info"
    extern proc c_string_to_real64(x: c_string) : real(64);

    select numBits(t) {
      when 32 do return c_string_to_real32(x.c_str());
      when 64 do return c_string_to_real64(x.c_str());
      otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
    }
  }

  inline proc _cast(type t, x: string) where isImagType(t) {
    pragma "insert line file info"
    extern proc c_string_to_imag32(x: c_string) : imag(32);
    pragma "insert line file info"
    extern proc c_string_to_imag64(x: c_string) : imag(64);

    select numBits(t) {
      when 32 do return c_string_to_imag32(x.c_str());
      when 64 do return c_string_to_imag64(x.c_str());
      otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
    }
  }


  //
  // complex
  //
  proc _cast(type t, x: complex(?w)) where t == string {
    if isnan(x.re) || isnan(x.im) then
      return "nan";
    var re = (x.re):string;
    var im: string;
    var op: string;
    if x.im < 0 {
      im = (-x.im):string;
      op = " - ";
    } else if x.im == -0.0 {
      im = "0.0";
      op = " - ";
    } else {
      im = (x.im):string;
      op = " + ";
    }
    const ts0 = re + op;
    const ts1 = ts0 + im;
    const ret = ts1 + "i";
    return ret;
  }


  inline proc _cast(type t, x: string) where isComplexType(t) {
    pragma "insert line file info"
    extern proc c_string_to_complex64(x:c_string) : complex(64);
    pragma "insert line file info"
    extern proc c_string_to_complex128(x:c_string) : complex(128);

    select numBits(t) {
      when 64 do return c_string_to_complex64(x.c_str());
      when 128 do return c_string_to_complex128(x.c_str());
      otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
    }
  }

  //
  // Catch all
  //
  // Convert 'x' to a string just the way it would be written out.
  // Includes Writer.write, with modifications (for simplicity; to avoid 'on').
  //
  // This is marked as compiler generated so it doesn't take precedence over
  // genereated casts for types like enums
  pragma "compiler generated"
  proc _cast(type t, x) where t == string {
    var ret: string;
    ret.write(x);
    return ret;
  }

  //
  // Badness
  //
  // TODO: I *really* dont like this
  // Other casts to strings use the bufferType
  //inline proc _cast(type t, x) where t == string && x.type != c_string && x.type != string {
  //  compilerError("_cast ":c_string+typeToString(x.type)+"->c_string->string":c_string);
  //}


}
