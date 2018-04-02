/*
 * Copyright 2004-2018 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

module StringCasts {
  use ChapelStandard;

  // TODO: I want to break all of these casts from string to T out into
  // T.parse(string), but we dont support methods on types yet. Ideally they
  // would use a tagged union return val as well.

  //
  // Type -- Foo.type:string
  //
  proc _cast(type t, type x)  param : string where t == string {
    return __primitive("typeToString", x);
  }

  //
  // Bool
  //

  inline proc _cast(type t, x: bool) where t == string {
    if (x) {
      return "true";
    } else {
      return "false";
    }
  }

  proc _cast(type t, x: string) throws where isBoolType(t) {
    var str = x.strip();
    if str.isEmptyString() {
      throw new IllegalArgumentError("bad cast from empty string to bool");
    } else if (str == "true") {
      return true;
    } else if (str == "false") {
      return false;
    } else {
      throw new IllegalArgumentError("bad cast from string '" + x + "' to bool");
    }
    return false;
  }

  //
  // int
  //
  proc _cast(type t, x: integral) where t == string {
    //TODO: switch to using qio's writef somehow
    extern proc integral_to_c_string(x:int(64), size:uint(32), isSigned: bool, ref err: bool) : c_string;
    extern proc strlen(const str: c_string) : size_t;

    var isErr: bool;
    var csc = integral_to_c_string(x:int(64), numBytes(x.type), isIntType(x.type), isErr);

    // this should only happen if the runtime is broken
    if isErr {
      try! {
        throw new IllegalArgumentError("Unexpected case in integral_to_c_string");
      }
    }

    var ret: string;
    ret.buff = csc:c_ptr(uint(8));
    ret.len = strlen(csc).safeCast(int);
    ret._size = ret.len+1;

    return ret;
  }

  inline proc _cast(type t, x: string) throws where isIntegralType(t) {
    //TODO: switch to using qio's readf somehow
    pragma "insert line file info"
    extern proc c_string_to_int8_t  (x:c_string, ref err: bool) : int(8);
    pragma "insert line file info"
    extern proc c_string_to_int16_t (x:c_string, ref err: bool) : int(16);
    pragma "insert line file info"
    extern proc c_string_to_int32_t (x:c_string, ref err: bool) : int(32);
    pragma "insert line file info"
    extern proc c_string_to_int64_t (x:c_string, ref err: bool) : int(64);
    pragma "insert line file info"
    extern proc c_string_to_uint8_t (x:c_string, ref err: bool) : uint(8);
    pragma "insert line file info"
    extern proc c_string_to_uint16_t(x:c_string, ref err: bool) : uint(16);
    pragma "insert line file info"
    extern proc c_string_to_uint32_t(x:c_string, ref err: bool) : uint(32);
    pragma "insert line file info"
    extern proc c_string_to_uint64_t(x:c_string, ref err: bool) : uint(64);

    var retVal: t;
    var isErr: bool;
    const localX = x.localize();

    if isIntType(t) {
      if localX.isEmptyString() then
        throw new IllegalArgumentError("bad cast from empty string to int(" + numBits(t) + ")");

      select numBits(t) {
        when 8  do retVal = c_string_to_int8_t(localX.c_str(), isErr);
        when 16 do retVal = c_string_to_int16_t(localX.c_str(), isErr);
        when 32 do retVal = c_string_to_int32_t(localX.c_str(), isErr);
        when 64 do retVal = c_string_to_int64_t(localX.c_str(), isErr);
        otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
      }

      if isErr then
        throw new IllegalArgumentError("bad cast from string '" + x + "' to int(" + numBits(t) + ")");
    } else {
      if localX.isEmptyString() then
        throw new IllegalArgumentError("bad cast from empty string to uint(" + numBits(t) + ")");

      select numBits(t) {
        when 8  do retVal = c_string_to_uint8_t(localX.c_str(), isErr);
        when 16 do retVal = c_string_to_uint16_t(localX.c_str(), isErr);
        when 32 do retVal = c_string_to_uint32_t(localX.c_str(), isErr);
        when 64 do retVal = c_string_to_uint64_t(localX.c_str(), isErr);
        otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
      }

      if isErr then
        throw new IllegalArgumentError("bad cast from string '" + x + "' to uint(" + numBits(t) + ")");
    }

    return retVal;
  }

  //
  // real & imag
  //
  inline proc _real_cast_helper(x: real(64), param isImag: bool) : string {
    extern proc real_to_c_string(x:real(64), isImag: bool) : c_string;
    extern proc strlen(const str: c_string) : size_t;

    var csc = real_to_c_string(x:real(64), isImag);

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

  inline proc _cast(type t, x: string) throws where isRealType(t) {
    pragma "insert line file info"
    extern proc c_string_to_real32(x: c_string, ref err: bool) : real(32);
    pragma "insert line file info"
    extern proc c_string_to_real64(x: c_string, ref err: bool) : real(64);

    var retVal: t;
    var isErr: bool;
    const localX = x.localize();

    if localX.isEmptyString() then
      throw new IllegalArgumentError("bad cast from empty string to real(" + numBits(t) + ")");

    select numBits(t) {
      when 32 do retVal = c_string_to_real32(localX.c_str(), isErr);
      when 64 do retVal = c_string_to_real64(localX.c_str(), isErr);
      otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
    }

    if isErr then
      throw new IllegalArgumentError("bad cast from string '" + x + "' to real(" + numBits(t) + ")");

    return retVal;
  }

  inline proc _cast(type t, x: string) throws where isImagType(t) {
    pragma "insert line file info"
    extern proc c_string_to_imag32(x: c_string, ref err: bool) : imag(32);
    pragma "insert line file info"
    extern proc c_string_to_imag64(x: c_string, ref err: bool) : imag(64);

    var retVal: t;
    var isErr: bool;
    const localX = x.localize();

    if localX.isEmptyString() then
      throw new IllegalArgumentError("bad cast from empty string to imag(" + numBits(t) + ")");

    select numBits(t) {
      when 32 do retVal = c_string_to_imag32(localX.c_str(), isErr);
      when 64 do retVal = c_string_to_imag64(localX.c_str(), isErr);
      otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
    }

    if isErr then
      throw new IllegalArgumentError("bad cast from string '" + x + "' to imag(" + numBits(t) + ")");

    return retVal;
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
    } else if x.im == -0.0 && -0.0 != 0.0 { // Special accommodation for Seymour.
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


  inline proc _cast(type t, x: string) throws where isComplexType(t) {
    pragma "insert line file info"
    extern proc c_string_to_complex64(x:c_string, ref err: bool) : complex(64);
    pragma "insert line file info"
    extern proc c_string_to_complex128(x:c_string, ref err: bool) : complex(128);

    var retVal: t;
    var isErr: bool;
    const localX = x.localize();

    if localX.isEmptyString() then
      throw new IllegalArgumentError("bad cast from empty string to complex(" + numBits(t) + ")");

    select numBits(t) {
      when 64 do retVal = c_string_to_complex64(localX.c_str(), isErr);
      when 128 do retVal = c_string_to_complex128(localX.c_str(), isErr);
      otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
    }

    if isErr then
      throw new IllegalArgumentError("bad cast from string '" + x + "' to complex(" + numBits(t) + ")");

    return retVal;
  }

  // Catch all cast anything -> string is in ChapelIO

}
