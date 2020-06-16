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

// ChapelHashing.chpl
//
// chpl__defaultHash and related functions
module ChapelHashing {

  private use ChapelBase;

  proc chpl__defaultHashWrapper(x): int {
    const hash = chpl__defaultHash(x);
    return (hash & max(int)): int;
  }

  // Mix the bits, so that e.g. numbers in 0..N generate
  // random-looking data across all the bits even if N is small.
  proc _gen_key(i: uint): uint {
    // Thomas Wang's 64b mix function - 2007 version - see
    // http://web.archive.org/web/20071223173210/http://www.concentric.net/~Ttwang/tech/inthash.htm
    var key = i;
    key = (~key) + (key << 21); // key = (key << 21) - key - 1;
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8); // key * 265
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4); // key * 21
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key;

    // See commit history for this comment for some other mixers
    // worth considering.
  }
  proc _gen_key(i: int): uint {
    return _gen_key(i:uint);
  }

  inline proc chpl__defaultHashCombine(a:uint, b:uint, fieldnum:int): uint {
    extern proc chpl_bitops_rotl_64(x: uint(64), n: uint(64)) : uint(64);
    var n:uint = (17 + fieldnum):uint;
    return _gen_key(a ^ chpl_bitops_rotl_64(b, n));
  }

  inline proc chpl__defaultHash(b: bool): uint {
    if (b) then
      return 0;
    else
      return 1;
  }

  inline proc chpl__defaultHash(i: int(64)): uint {
    return _gen_key(i);
  }

  inline proc chpl__defaultHash(u: uint(64)): uint {
    return _gen_key(u);
  }

  inline proc chpl__defaultHash(e) where isEnum(e) {
    return _gen_key(chpl__enumToOrder(e));
  }

  inline proc chpl__defaultHash(f: real): uint {
    return _gen_key(__primitive( "real2int", f));
  }

  inline proc chpl__defaultHash(c: complex): uint {
    return _gen_key(__primitive("real2int", c.re) ^ __primitive("real2int", c.im));
  }

  inline proc chpl__defaultHash(a: imag): uint {
    return _gen_key(__primitive( "real2int", _i2r(a)));
  }

  inline proc chpl__defaultHash(u: chpl_taskID_t): uint {
    return _gen_key(u:int(64));
  }

  inline proc chpl__defaultHash(l : []): uint {
    var hash : uint = 0;
    var i = 1;
    for obj in l {
      hash = chpl__defaultHashCombine(chpl__defaultHash(obj), hash, i);
      i += 1;
    }
    return hash;
  }

  // Nilable and non-nilable classes will coerce to this.
  inline proc chpl__defaultHash(o: borrowed object?): uint {
    return _gen_key(__primitive( "object2int", o));
  }

  inline proc chpl__defaultHash(l: locale): uint {
    return _gen_key(__primitive( "object2int", l._value));
  }

  //
  // Implementation of chpl__defaultHash for ranges, in case the 'keyType'
  // contains a range in some way (e.g. tuple of ranges).
  //
  inline proc chpl__defaultHash(r : range): uint {
    use Reflection;
    var ret : uint;
    for param i in 1..numImplementationFields(r.type) {
      if isParam(getImplementationField(r, i)) == false &&
         isType(getImplementationField(r, i)) == false &&
         isNothingType(getImplementationField(r, i).type) == false {
        const ref field = getImplementationField(r, i);
        const fieldHash = chpl__defaultHash(field);
        if i == 1 then
          ret = fieldHash;
        else
          ret = chpl__defaultHashCombine(fieldHash, ret, i);
      }
    }
    return ret;
  }

  // Is 'idxType' legal to create a default associative domain with?
  // Currently based on the availability of chpl__defaultHash().
  // Enumerated and sparse domains are handled separately.
  // Tuples and records also work, somehow.
  proc chpl__validDefaultAssocDomIdxType(type idxType) param return false;

  proc chpl__validDefaultAssocDomIdxType(type idxType) param where
      // one check per an implementation of chpl__defaultHash() above
      isBoolType(idxType)     ||
      isIntType(idxType)      ||
      isUintType(idxType)    ||
      isRealType(idxType)        ||
      isImagType(idxType)        ||
      isComplexType(idxType)     ||
      idxType == chpl_taskID_t    ||
      idxType == string           ||
      idxType == bytes            ||
      idxType == c_string         ||
      isClassType(idxType)        ||
      // these are handled differently
      isEnumType(idxType)  ||
      isTupleType(idxType)        ||
      isRecordType(idxType)
  {
    return true;
  }

}
