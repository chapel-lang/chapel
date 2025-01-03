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

// ChapelHashing.chpl
//
// hash and related functions
module ChapelHashing {

  use ChapelBase;

  interface hashable {
    proc Self.hash(): uint;
  }

  // Hash interfaces return uint, this internal implementation (also used for
  // compiler-generated hashes) also returns uint. The public API returns
  // integers, though, so is a different function.
  proc chpl__defaultHashWrapperInner(x): uint {
    use Reflection;

    if !canResolveMethod(x, "hash") {
      compilerError("No hash function found for " + x.type:string);
    } else if __primitive("implements interface", x, hashable) == 2 {
      compilerWarning("'", x.type:string + "' has a hash function that is being ",
                      "used by the standard library. However, '" + x.type:string +
                      "' does not implement hashable. ",
                      "In the future, this will result in an error.");
      if isRecordType(x.type) {
        compilerWarning("to make '" + x.type:string + "' implement hashable, ",
                        "add the interface to its declaration: 'record " + x.type:string +
                        " : hashable'");
      }
    }

    return x.hash();
  }

  proc chpl__defaultHashWrapper(x): int {
    const hash = chpl__defaultHashWrapperInner(x);
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
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_bitops_rotl_64(x: uint(64), n: uint(64)) : uint(64);
    var n:uint = (17 + fieldnum):uint;
    return _gen_key(a ^ chpl_bitops_rotl_64(b, n));
  }

  inline proc bool.hash(): uint {
    if (this) then
      return 0;
    else
      return 1;
  }
  bool implements hashable;

  // The hash methods for various numeric types are below.
  implements hashable(numeric);

  inline proc int.hash(): uint {
    return _gen_key(this);
  }

  inline proc uint.hash(): uint {
    return _gen_key(this);
  }

  inline proc enum.hash(): uint {
    return _gen_key(chpl__enumToOrder(this));
  }
  implements hashable(enum);

  inline proc real.hash(): uint {
    return _gen_key(__primitive( "real2int", this));
  }

  inline proc complex.hash(): uint {
    return _gen_key(__primitive("real2int", this.re) ^ __primitive("real2int", this.im));
  }

  inline proc imag.hash(): uint {
    return _gen_key(__primitive( "real2int", _i2r(this)));
  }

  inline proc chpl_taskID_t.hash(): uint {
    return _gen_key(this:int);
  }
  chpl_taskID_t implements hashable;

  inline proc _array.hash(): uint {
    var hash : uint = 0;
    var i = 1;
    for obj in this {
      hash = chpl__defaultHashCombine(obj.hash(), hash, i);
      i += 1;
    }
    return hash;
  }
  _array implements hashable;

  // Nilable and non-nilable classes will coerce to this.
  inline proc (borrowed RootClass?).hash(): uint {
    return _gen_key(__primitive( "object2int", this));
  }
  implements hashable(class);
  implements hashable(class?);

  inline proc locale.hash(): uint {
    return _gen_key(__primitive( "object2int", this._value));
  }
  locale implements hashable;

  //
  // Implementation of hash for ranges, in case the 'keyType'
  // contains a range in some way (e.g. tuple of ranges).
  //
  inline proc range.hash(): uint {
    use Reflection;
    var ret : uint;
    for param i in 1..numImplementationFields(this.type) {
      if isParam(getImplementationField(this, i)) == false &&
         isType(getImplementationField(this, i)) == false &&
         isNothingType(getImplementationField(this, i).type) == false {
        const ref field = getImplementationField(this, i);
        const fieldHash = field.hash();
        if i == 1 then
          ret = fieldHash;
        else
          ret = chpl__defaultHashCombine(fieldHash, ret, i);
      }
    }
    return ret;
  }
  range implements hashable;
}
