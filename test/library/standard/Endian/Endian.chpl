/*Documentation for the module*/
module Endian {
  use SysCTypes;

  private extern proc htole32(x: c_uint): c_uint;
  private extern proc le32toh(x: c_uint): c_uint;
  private extern proc htobe32(x: c_uint): c_uint;
  private extern proc be32toh(x: c_uint): c_uint;
  private extern proc htole64(x: c_ulong): c_ulong;
  private extern proc le64toh(x: c_ulong): c_ulong;
  private extern proc htobe64(x: c_ulong): c_ulong;
  private extern proc be64toh(x: c_ulong): c_ulong;

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
