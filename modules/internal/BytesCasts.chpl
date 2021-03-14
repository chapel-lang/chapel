/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

module BytesCasts {
  private use ChapelStandard;
  private use BytesStringCommon;
  private use SysCTypes;
  private use CPtr;


  //
  // Bool
  //
  operator :(x: bool, type t:bytes) {
    if (x) {
      return b"true";
    } else {
      return b"false";
    }
  }

  operator :(x: bytes, type t:chpl_anybool) throws {
    var b = x.strip();
    if b.isEmpty() {
      // TODO engin: do we really need this check?
      throw new owned IllegalArgumentError("bad cast from empty bytes to bool");
    } else if (b == b"true") {
      return true;
    } else if (b == b"false") {
      return false;
    } else {
      throw new owned IllegalArgumentError("bad cast from bytes '" +
                                           x.decode(decodePolicy.drop) +
                                           "' to bool");
    }
    return false;
  }

  //
  // int
  //
  operator :(x: integral, type t:bytes) {
    //TODO: switch to using qio's writef somehow
    pragma "fn synchronization free"
    extern proc integral_to_c_string(x:int(64), size:uint(32), isSigned: bool,
                                     ref err: bool) : c_string;
    pragma "fn synchronization free"
    extern proc strlen(const str: c_string) : size_t;

    var isErr: bool;
    var csc = integral_to_c_string(x:int(64), numBytes(x.type),
                                   isIntType(x.type), isErr);

    // this should only happen if the runtime is broken
    if isErr {
      try! {
        throw new owned
          IllegalArgumentError("Unexpected case in integral_to_c_string");
      }
    }

    var ret: bytes;
    ret.buff = csc:c_ptr(uint(8));
    ret.buffLen = strlen(csc).safeCast(int);
    ret.buffSize = ret.buffLen+1;

    return ret;
  }

  operator :(x: bytes, type t:integral) throws {
    //TODO: switch to using qio's readf somehow
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_int8_t  (x:c_string, ref err: bool) : int(8);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_int16_t (x:c_string, ref err: bool) : int(16);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_int32_t (x:c_string, ref err: bool) : int(32);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_int64_t (x:c_string, ref err: bool) : int(64);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_uint8_t (x:c_string, ref err: bool) : uint(8);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_uint16_t(x:c_string, ref err: bool) : uint(16);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_uint32_t(x:c_string, ref err: bool) : uint(32);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_uint64_t(x:c_string, ref err: bool) : uint(64);

    var retVal: t;
    var isErr: bool;
    // localize the bytes and remove leading and trailing whitespace
    var localX = x.localize();

    if localX.isEmpty() then
      throw new owned IllegalArgumentError("bad cast from empty bytes to " +
                                           t:string);
    _cleanupForNumericCast(localX);

    if localX.isEmpty() then
      throw new owned IllegalArgumentError("bad cast from bytes '" +
                                           x.decode(decodePolicy.drop) +
                                           "' to " + t:string);

    if isIntType(t) {
      select numBits(t) {
        when 8  do retVal = c_string_to_int8_t(localX.c_str(), isErr);
        when 16 do retVal = c_string_to_int16_t(localX.c_str(), isErr);
        when 32 do retVal = c_string_to_int32_t(localX.c_str(), isErr);
        when 64 do retVal = c_string_to_int64_t(localX.c_str(), isErr);
        otherwise compilerError("Unsupported bit width ", numBits(t),
                                " in cast from bytes to " + t:string);
      }
    } else {
      select numBits(t) {
        when 8  do retVal = c_string_to_uint8_t(localX.c_str(), isErr);
        when 16 do retVal = c_string_to_uint16_t(localX.c_str(), isErr);
        when 32 do retVal = c_string_to_uint32_t(localX.c_str(), isErr);
        when 64 do retVal = c_string_to_uint64_t(localX.c_str(), isErr);
        otherwise compilerError("Unsupported bit width ", numBits(t),
                                " in cast from bytes to " + t:string);
      }
    }

    if isErr then
      throw new owned IllegalArgumentError("bad cast from bytes '" +
                                           x.decode(decodePolicy.drop) +
                                           "' to " + t:string);

    return retVal;
  }

  //
  // real & imag
  //
  proc _real_cast_helper(x: real(64), param isImag: bool) : bytes {
    pragma "fn synchronization free"
    extern proc real_to_c_string(x:real(64), isImag: bool) : c_string;
    pragma "fn synchronization free"
    extern proc strlen(const str: c_string) : size_t;

    var csc = real_to_c_string(x:real(64), isImag);

    var ret: bytes;
    ret.buff = csc:c_ptr(uint(8));
    ret.buffLen = strlen(csc).safeCast(int);
    ret.buffSize = ret.buffLen+1;

    return ret;
  }

  inline operator :(x:chpl_anyreal, type t:bytes) {
    //TODO: switch to using qio's writef somehow
    return _real_cast_helper(x:real(64), false);
  }

  operator :(x:chpl_anyimag, type t:bytes) {
    //TODO: switch to using qio's writef somehow
    // The Chapel version of the imag --> real cast smashes it flat rather than
    // just stripping off the "i".  See the cast in ChapelBase.
    var r = __primitive("cast", real(64), x);
    return _real_cast_helper(r, true);
  }

  operator :(x: bytes, type t:chpl_anyreal) throws {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_real32(x: c_string, ref err: bool) : real(32);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_real64(x: c_string, ref err: bool) : real(64);

    var retVal: t;
    var isErr: bool;
    var localX = x.localize();

    _cleanupForNumericCast(localX);

    select numBits(t) {
      when 32 do retVal = c_string_to_real32(localX.c_str(), isErr);
      when 64 do retVal = c_string_to_real64(localX.c_str(), isErr);
      otherwise compilerError("Unsupported bit width ", numBits(t),
                              " in cast to bytes");
    }

    if isErr then
      throw new owned IllegalArgumentError("bad cast from bytes '" +
                                           x.decode(decodePolicy.drop) +
                                           "' to real(" + numBits(t):string + ")");

    return retVal;
  }

  operator :(x: bytes, type t:chpl_anyimag) throws {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_imag32(x: c_string, ref err: bool) : imag(32);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_imag64(x: c_string, ref err: bool) : imag(64);

    var retVal: t;
    var isErr: bool;
    var localX = x.localize();

    _cleanupForNumericCast(localX);

    select numBits(t) {
      when 32 do retVal = c_string_to_imag32(localX.c_str(), isErr);
      when 64 do retVal = c_string_to_imag64(localX.c_str(), isErr);
      otherwise compilerError("Unsupported bit width ", numBits(t),
                              " in cast to bytes");
    }

    if isErr then
      throw new owned IllegalArgumentError("bad cast from bytes '" +
                                           x.decode(decodePolicy.drop) +
                                           "' to imag(" + numBits(t):string + ")");

    return retVal;
  }

  //
  // complex
  //
  operator :(x: chpl_anycomplex, type t:bytes) {
    if isnan(x.re) || isnan(x.im) then
      return b"nan";
    var re = (x.re):bytes;
    var im: bytes;
    var op: bytes;
    if x.im < 0 {
      im = (-x.im):bytes;
      op = " - ";
    } else if x.im == -0.0 && -0.0 != 0.0 { // Special accommodation for Seymour.
      im = "0.0";
      op = " - ";
    } else {
      im = (x.im):bytes;
      op = " + ";
    }
    const ts0 = re + op;
    const ts1 = ts0 + im;
    const ret = ts1 + b"i";
    return ret;
  }


  operator :(x: bytes, type t:chpl_anycomplex) throws {
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_complex64(x:c_string, ref err: bool) : complex(64);
    pragma "fn synchronization free"
    pragma "insert line file info"
    extern proc c_string_to_complex128(x:c_string, ref err: bool) : complex(128);

    var retVal: t;
    var isErr: bool;
    const localX = x.localize();

    if localX.isEmpty() then
      throw new owned 
        IllegalArgumentError("bad cast from empty bytes to complex(" +
                             numBits(t):string + ")");

    select numBits(t) {
      when 64 do retVal = c_string_to_complex64(localX.c_str(), isErr);
      when 128 do retVal = c_string_to_complex128(localX.c_str(), isErr);
      otherwise compilerError("Unsupported bit width ", numBits(t),
                              " in cast to bytes");
    }

    if isErr then
      throw new owned IllegalArgumentError("bad cast from bytes '" +
                                           x.decode(decodePolicy.drop) +
                                           "' to complex(" + numBits(t):string + ")");

    return retVal;
  }
} // end of module BytesCasts
