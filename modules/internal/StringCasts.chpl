/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
  private use ChapelStandard;
  private use BytesStringCommon;
  private use CTypes;
  private use String.NVStringFactory;

  // TODO: I want to break all of these casts from string to T out into
  // T.parse(string), but we dont support methods on types yet. Ideally they
  // would use a tagged union return val as well.

  // SomeType:string or Foo.type:string is handled directly by compiler.

  //
  // Bool
  //

  operator :(x: bool, type t:string) {
    if (x) {
      return "true";
    } else {
      return "false";
    }
  }

  operator :(x: string, type t:bool) throws {
    var str = x.strip();
    if str.isEmpty() {
      throw new owned IllegalArgumentError("bad cast from empty string to bool");
    } else if (str == "true") {
      return true;
    } else if (str == "false") {
      return false;
    } else {
      throw new owned IllegalArgumentError("bad cast from string '" + x + "' to bool");
    }
    return false;
  }

  // homogeneous tuples of primitive type
  operator :(x: ?k*?t, type s:string) where isPrimitiveType(t) && isHomogeneousTupleType(x.type) {
    var ret = "(";
    for param i in 0..#k {
      if i != 0 then ret += ", ";
      ret += x[i]:string;
    }
    if k == 1 then ret += ",";
    ret += ")";
    return ret;
  }

  //
  // int
  //
  operator :(x: integral, type t:string) {
    //TODO: switch to using qio's writef somehow
    pragma "fn synchronization free"
    extern proc integral_to_c_string(x:int(64), size:uint(32), isSigned: bool, ref err: bool) : c_ptrConst(c_char);
    pragma "fn synchronization free"
    extern proc strlen(const str: c_ptrConst(c_char)) : c_size_t;

    var isErr: bool;
    var csc = integral_to_c_string(x:int(64), numBytes(x.type), isIntType(x.type), isErr);

    // this should only happen if the runtime is broken
    if isErr {
      try! {
        throw new owned IllegalArgumentError("Unexpected case in integral_to_c_string");
      }
    }

    const len = strlen(csc).safeCast(int);
    return chpl_createStringWithOwnedBufferNV(x=csc:c_ptr(uint(8)),
                                              length=len,
                                              size=len+1,
                                              numCodepoints=len);
  }

  operator :(x: string, type t:integral) throws {
    //TODO: switch to using qio's readf somehow
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_int8_t  (x:c_ptrConst(c_char), ref err: bool) : int(8);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_int16_t (x:c_ptrConst(c_char), ref err: bool) : int(16);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_int32_t (x:c_ptrConst(c_char), ref err: bool) : int(32);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_int64_t (x:c_ptrConst(c_char), ref err: bool) : int(64);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_uint8_t (x:c_ptrConst(c_char), ref err: bool) : uint(8);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_uint16_t(x:c_ptrConst(c_char), ref err: bool) : uint(16);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_uint32_t(x:c_ptrConst(c_char), ref err: bool) : uint(32);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_uint64_t(x:c_ptrConst(c_char), ref err: bool) : uint(64);

    var retVal: t;
    var isErr: bool;
    // localize the string and remove leading and trailing whitespace
    var localX = x.localize();

    if localX.isEmpty() then
      throw new owned IllegalArgumentError("bad cast from empty string to " +
                                           t:string);

    _cleanupForNumericCast(localX);

    if localX.isEmpty() then
      throw new owned IllegalArgumentError("bad cast from string '"+ x + "' to " + t:string);

    if isIntType(t) {
      select numBits(t) {
        when 8  do retVal = c_string_to_int8_t(localX.c_str(), isErr);
        when 16 do retVal = c_string_to_int16_t(localX.c_str(), isErr);
        when 32 do retVal = c_string_to_int32_t(localX.c_str(), isErr);
        when 64 do retVal = c_string_to_int64_t(localX.c_str(), isErr);
        otherwise compilerError("Unsupported bit width ", numBits(t), " in cast from string to " + t:string);
      }
    } else {
      select numBits(t) {
        when 8  do retVal = c_string_to_uint8_t(localX.c_str(), isErr);
        when 16 do retVal = c_string_to_uint16_t(localX.c_str(), isErr);
        when 32 do retVal = c_string_to_uint32_t(localX.c_str(), isErr);
        when 64 do retVal = c_string_to_uint64_t(localX.c_str(), isErr);
        otherwise compilerError("Unsupported bit width ", numBits(t), " in cast from string to " + t:string);
      }
    }

    if isErr then
      throw new owned IllegalArgumentError("bad cast from string '" + x + "' to " + t:string);

    return retVal;
  }

  //
  // real & imag
  //
  proc _real_cast_helper(x: real(64), param isImag: bool) : string {
    pragma "fn synchronization free"
    extern proc real_to_c_string(x:real(64), isImag: bool) : c_ptrConst(c_char);
    pragma "fn synchronization free"
    extern proc strlen(const str: c_ptrConst(c_char)) : c_size_t;

    var csc = real_to_c_string(x:real(64), isImag);

    const len = strlen(csc).safeCast(int);
    return chpl_createStringWithOwnedBufferNV(x=csc:c_ptr(uint(8)),
                                              length=len,
                                              size=len+1,
                                              numCodepoints=len);
  }

  inline operator :(x:chpl_anyreal, type t:string) {
    //TODO: switch to using qio's writef somehow
    return _real_cast_helper(x:real(64), false);
  }

  operator :(x:chpl_anyimag, type t:string) {
    //TODO: switch to using qio's writef somehow
    // The Chapel version of the imag --> real cast smashes it flat rather than
    // just stripping off the "i".  See the cast in ChapelBase.
    var r = __primitive("cast", real(64), x);
    return _real_cast_helper(r, true);
  }

  operator :(x: string, type t:chpl_anyreal) throws {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_real32(x: c_ptrConst(c_char), ref err: bool) : real(32);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_real64(x: c_ptrConst(c_char), ref err: bool) : real(64);

    var retVal: t;
    var isErr: bool;
    var localX = x.localize();

    if localX.isEmpty() then
      throw new owned IllegalArgumentError("bad cast from empty string to " +
                                           t:string);

    _cleanupForNumericCast(localX);

    select numBits(t) {
      when 32 do retVal = c_string_to_real32(localX.c_str(), isErr);
      when 64 do retVal = c_string_to_real64(localX.c_str(), isErr);
      otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
    }

    if isErr then
      throw new owned IllegalArgumentError("bad cast from string '" + x + "' to real(" + numBits(t):string + ")");

    return retVal;
  }

  operator :(x: string, type t:chpl_anyimag) throws {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_imag32(x: c_ptrConst(c_char), ref err: bool) : imag(32);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_imag64(x: c_ptrConst(c_char), ref err: bool) : imag(64);

    var retVal: t;
    var isErr: bool;
    var localX = x.localize();

    if localX.isEmpty() then
      throw new owned IllegalArgumentError("bad cast from empty string to " +
                                           t:string);

    _cleanupForNumericCast(localX);

    select numBits(t) {
      when 32 do retVal = c_string_to_imag32(localX.c_str(), isErr);
      when 64 do retVal = c_string_to_imag64(localX.c_str(), isErr);
      otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
    }

    if isErr then
      throw new owned IllegalArgumentError("bad cast from string '" + x + "' to imag(" + numBits(t):string + ")");

    return retVal;
  }


  //
  // complex
  //
  @edition(first="preview")
  operator :(x: chpl_anycomplex, type t:string) {
    return chpl_fromComplexCastHelper(x, t);
  }

  @edition(last="2.0")
  operator :(x: chpl_anycomplex, type t:string) {
    if isNan(x.re) || isNan(x.im) then
      return "nan";
    return chpl_fromComplexCastHelper(x, t);
  }


operator :(x: string, type t:chpl_anycomplex) throws {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_complex64(x:c_ptrConst(c_char), ref err: bool) : complex(64);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_complex128(x:c_ptrConst(c_char), ref err: bool) : complex(128);

    var retVal: t;
    var isErr: bool;
    const localX = x.localize();

    if localX.isEmpty() then
      throw new owned IllegalArgumentError("bad cast from empty string to complex(" + numBits(t):string + ")");

    select numBits(t) {
      when 64 do retVal = c_string_to_complex64(localX.c_str(), isErr);
      when 128 do retVal = c_string_to_complex128(localX.c_str(), isErr);
      otherwise compilerError("Unsupported bit width ", numBits(t), " in cast to string");
    }

    if isErr then
      throw new owned IllegalArgumentError("bad cast from string '" + x + "' to complex(" + numBits(t):string + ")");

    return retVal;
  }
}
