/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

 /*
    Supports for little-endian and big-endian byte order conversions.

    This module provides a Chapel interface wrapped over the standard
    C endian module to support little-endian and big-endian byte order
    conversions. It has functions to convert byte encoding of integer
    values from the byte order of the host to and from little-endian
    and big-endian formats.
 */
module Endian {
  use SysCTypes;

  private extern proc htole16(x: c_uint): c_uint;
  private extern proc le16toh(x: c_uint): c_uint;
  private extern proc htobe16(x: c_uint): c_uint;
  private extern proc be16toh(x: c_uint): c_uint;
  private extern proc htole32(x: c_uint): c_uint;
  private extern proc le32toh(x: c_uint): c_uint;
  private extern proc htobe32(x: c_uint): c_uint;
  private extern proc be32toh(x: c_uint): c_uint;
  private extern proc htole64(x: c_ulong): c_ulong;
  private extern proc le64toh(x: c_ulong): c_ulong;
  private extern proc htobe64(x: c_ulong): c_ulong;
  private extern proc be64toh(x: c_ulong): c_ulong;

  /* Returns the little endian order of the argument.

      :arg x: a 16-bit signed integer or a 16-bit unsigned integer
  */
  proc hostToLittleEndian(x) where numBytes(x.type) == 2 {
    var a = x;
    var b: uint(16);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = htole16(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  /* Returns the host byte order of the argument.

      :arg x: a 16-bit signed integer or a 16-bit unsigned integer
  */
  proc littleEndianToHost(x) where numBytes(x.type) == 2 {
    var a = x;
    var b: uint(16);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = le16toh(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  /* Returns the big endian order of the argument.

      :arg x: a 16-bit signed integer or a 16-bit unsigned integer
  */
  proc hostToBigEndian(x) where numBytes(x.type) == 2 {
    var a = x;
    var b: uint(16);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = htobe16(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  /* Returns the host byte order of the argument.

      :arg x: a 16-bit signed integer or a 16-bit unsigned integer
  */
  proc bigEndianToHost(x) where numBytes(x.type) == 2 {
    var a = x;
    var b: uint(16);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = be16toh(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  /* Returns the little endian order of the argument.

      :arg x: a 32-bit signed integer, a 32-bit unsigned integer
              or a 32-bit real.
  */
  proc hostToLittleEndian(x) where numBytes(x.type) == 4 {
    var a = x;
    var b: uint(32);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = htole32(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  /* Returns the host byte order of the argument.

      :arg x: a 32-bit signed integer, a 32-bit unsigned integer
              or a 32-bit real.
  */
  proc littleEndianToHost(x) where numBytes(x.type) == 4 {
    var a = x;
    var b: uint(32);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = le32toh(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  /* Returns the big endian order of the argument.

      :arg x: a 32-bit signed integer, a 32-bit unsigned integer
              or a 32-bit real.
  */
  proc hostToBigEndian(x) where numBytes(x.type) == 4 {
    var a = x;
    var b: uint(32);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = htobe32(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  /* Returns the host byte order of the argument.

      :arg x: a 32-bit signed integer, a 32-bit unsigned integer
              or a 32-bit real.
  */
  proc bigEndianToHost(x) where numBytes(x.type) == 4 {
    var a = x;
    var b: uint(32);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = be32toh(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  /* Returns the little endian order of the argument.

      :arg x: a 64-bit signed integer, a 64-bit unsigned integer
              or a 64-bit real.
  */
  proc hostToLittleEndian(x) where numBytes(x.type) == 8 {
    var a = x;
    var b: uint(64);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = htole64(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  /* Returns the host byte order of the argument.

      :arg x: a 64-bit signed integer, a 64-bit unsigned integer
              or a 64-bit real.
  */
  proc littleEndianToHost(x) where numBytes(x.type) == 8 {
    var a = x;
    var b: uint(64);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = le64toh(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  /* Returns the big endian order of the argument.

      :arg x: a 64-bit signed integer, a 64-bit unsigned integer
              or a 64-bit real.
  */
  proc hostToBigEndian(x) where numBytes(x.type) == 8 {
    var a = x;
    var b: uint(64);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = htobe64(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  /* Returns the host byte order of the argument.

      :arg x: a 64-bit signed integer, a 64-bit unsigned integer
              or a 64-bit real.
  */
  proc bigEndianToHost(x) where numBytes(x.type) == 8 {
    var a = x;
    var b: uint(64);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = be64toh(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }
    
}
