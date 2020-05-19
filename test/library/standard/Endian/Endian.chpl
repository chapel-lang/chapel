module Endian {
  use SysCTypes;

  extern proc htole32(x: c_uint): c_uint;
  extern proc le32toh(x: c_uint): c_uint;
  extern proc htobe32(x: c_uint): c_uint;
  extern proc be32toh(x: c_uint): c_uint;
  extern proc htole64(x: c_ulong): c_ulong;
  extern proc le64toh(x: c_ulong): c_ulong;
  extern proc htobe64(x: c_ulong): c_ulong;
  extern proc be64toh(x: c_ulong): c_ulong;

  proc hostToLittleEndian(x: uint(32)): uint(32) {
    return htole32(x);
  }

  proc littleEndianToHost(x: uint(32)): uint(32) {
    return le32toh(x);
  }

  proc hostToBigEndian(x: uint(32)): uint(32) {
    return htobe32(x);
  }

  proc bigEndianToHost(x: uint(32)): uint(32) {
    return be32toh(x);
  }

  proc hostToLittleEndian(x: uint(64)): uint(64) {
    return htole64(x);
  }

  proc littleEndianToHost(x: uint(64)): uint(64) {
    return le64toh(x);
  }

  proc hostToBigEndian(x: uint(64)): uint(64) {
    return htobe64(x);
  }

  proc bigEndianToHost(x: uint(64)): uint(64) {
    return be64toh(x);
  }

  proc hostToLittleEndian(x) where numBytes(x.type) == 4 {
    var a = x;
    var b: uint(32);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = htole32(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  proc littleEndianToHost(x) where numBytes(x.type) == 4 {
    var a = x;
    var b: uint(32);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = le32toh(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  proc hostToBigEndian(x) where numBytes(x.type) == 4 {
    var a = x;
    var b: uint(32);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = htobe32(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  proc bigEndianToHost(x) where numBytes(x.type) == 4 {
    var a = x;
    var b: uint(32);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = be32toh(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  proc hostToLittleEndian(x) where numBytes(x.type) == 8 {
    var a = x;
    var b: uint(64);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = htole64(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  proc littleEndianToHost(x) where numBytes(x.type) == 8 {
    var a = x;
    var b: uint(64);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = le64toh(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  proc hostToBigEndian(x) where numBytes(x.type) == 8 {
    var a = x;
    var b: uint(64);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = htobe64(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }

  proc bigEndianToHost(x) where numBytes(x.type) == 8 {
    var a = x;
    var b: uint(64);
    c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
    var ret = be64toh(b);
    c_memcpy(c_ptrTo(a), c_ptrTo(ret), c_sizeof(a.type));
    return a;
  }
    
}