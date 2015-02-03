module StringCasts {
  // TODO: any way to make this work?
  // cant have string constants outside of functions, we run into errors as
  // ChapelLocale hasnt been parsed yet.
  //const _true_s: string = "true";
  //const _false_s: string = "false";

  //
  // Bool
  //
  proc _cast(type t, x: bool) where t == string {
    if (x) {
      const _true_s: string = "true";
      return _true_s;
    } else {
      const _false_s: string = "false";
      return _false_s;
    }
  }

  proc _cast(type t, x: string) where t == bool {
      const _true_s: string = "true";
      const _false_s: string = "false";
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
  proc _cast(type t, x:integral) where t == string {
    //TODO: switch to using qio's writef somehow
    //TODO: I think this might be broken anyways? cast to int64 feels wrong
    extern proc integral_to_c_string_copy(x:int(64), size:uint(32), isSigned: bool) : c_string_copy ;
    extern proc strlen(const str: c_string_copy) : size_t;

    var csc = integral_to_c_string_copy(x:int(64), numBytes(x.type), isIntType(x.type));

    var ret: string;
    ret.base = csc:c_ptr(uint(8));
    ret.len = strlen(csc);
    ret._size = ret.len+1;

    return ret;
  }

  //
  // real & imag
  //
  inline proc _real_cast_helper(x: real(64), param isImag: bool) : string {
    extern proc real_to_c_string_copy(x:real(64), isImag: bool) : c_string_copy;
    extern proc strlen(const str: c_string_copy) : size_t;

    var csc = real_to_c_string_copy(x:real(64), isImag);

    var ret: string;
    ret.base = csc:c_ptr(uint(8));
    ret.len = strlen(csc);
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

  //
  // Badness
  //
  // TODO: I *really* dont like this
  // Other casts to strings use the baseType
  inline proc _cast(type t, x) where t == string && x.type != c_string && x.type != string {
    compilerError("_cast ":c_string+typeToString(x.type)+"->c_string->string":c_string);
  }


}
