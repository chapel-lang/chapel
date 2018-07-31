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

module BufferedAtomics {

  private proc externFunc(param s: string, type T) param {
    if isInt(T)  then return "chpl_comm_atomic_" + s + "_int"  + numBits(T):string;
    if isUint(T) then return "chpl_comm_atomic_" + s + "_uint" + numBits(T):string;
    if isReal(T) then return "chpl_comm_atomic_" + s + "_real" + numBits(T):string;
  }

  inline proc RAtomicT.addBuff(value:T): void {
    pragma "insert line file info" extern externFunc("add_buff", T)
      proc atomic_add_buff(ref op:T, l:int(32), ref obj:T): void;

    var v = value;
    atomic_add_buff(v, _localeid(), _v);
  }

  inline proc RAtomicT.subBuff(value:T): void {
    pragma "insert line file info" extern externFunc("sub_buff", T)
      proc atomic_sub_buff(ref op:T, l:int(32), ref obj:T): void;

    var v = value;
    atomic_sub_buff(v, _localeid(), _v);
  }

  inline proc RAtomicT.orBuff(value:T): void {
    if !isIntegral(T) then compilerError("or is only defined for integer atomic types");
    pragma "insert line file info" extern externFunc("or_buff", T)
      proc atomic_or_buff(ref op:T, l:int(32), ref obj:T): void;

    var v = value;
    atomic_or_buff(v, _localeid(), _v);
  }

  inline proc RAtomicT.andBuff(value:T): void {
    if !isIntegral(T) then compilerError("and is only defined for integer atomic types");
    pragma "insert line file info" extern externFunc("and_buff", T)
      proc atomic_and_buff(ref op:T, l:int(32), ref obj:T): void;

    var v = value;
    atomic_and_buff(v, _localeid(), _v);
  }

  inline proc RAtomicT.xorBuff(value:T): void {
    if !isIntegral(T) then compilerError("xor is only defined for integer atomic types");
    pragma "insert line file info" extern externFunc("xor_buff", T)
      proc atomic_xor_buff(ref op:T, l:int(32), ref obj:T): void;

    var v = value;
    atomic_xor_buff(v, _localeid(), _v);
  }

  inline proc atomicBuffFlush(): void {
    if CHPL_NETWORK_ATOMICS != "none" {
      extern proc chpl_comm_atomic_buff_flush();
      coforall loc in Locales do on loc {
        chpl_comm_atomic_buff_flush();
      }
    }
  }
}
