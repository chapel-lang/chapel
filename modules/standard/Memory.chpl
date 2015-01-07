/*
 * Copyright 2004-2015 Cray Inc.
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

pragma "insert line file info" extern proc chpl_memoryUsed(): uint(64);

enum MemUnits {Bytes, KB, MB, GB};

proc locale.physicalMemory(unit: MemUnits=MemUnits.Bytes, type retType=int(64)) {
  extern proc chpl_bytesPerLocale(): uint(64);

  var bytesInLocale: uint(64);

  on this do bytesInLocale = chpl_bytesPerLocale();

  var retVal: retType;
  select (unit) {
    when MemUnits.Bytes do retVal = bytesInLocale:retType;
    when MemUnits.KB do retVal = (bytesInLocale:retType / 1024):retType;
    when MemUnits.MB do retVal = (bytesInLocale:retType / (1024**2)):retType;
    when MemUnits.GB do retVal = (bytesInLocale:retType / (1024**3)):retType;
  }

  return retVal;
}

proc memoryUsed() {
  return chpl_memoryUsed();
}

proc printMemTable(thresh=0) {
  pragma "insert line file info" 
  extern proc chpl_printMemTable(thresh);

  chpl_printMemTable(thresh);
}

proc printMemLeaksTable() {
  extern proc chpl_printLeakedMemTable();
  chpl_printLeakedMemTable();
}

proc printMemStat() {
  pragma "insert line file info"
  extern proc chpl_printMemStat();

  chpl_printMemStat();
}

proc startVerboseMem() { 
  extern proc chpl_startVerboseMem();
  chpl_startVerboseMem();
}

proc stopVerboseMem() {
  extern proc chpl_stopVerboseMem();
  chpl_stopVerboseMem();
}

proc startVerboseMemHere() {
  extern proc chpl_startVerboseMemHere();
  chpl_startVerboseMemHere();
}

proc stopVerboseMemHere() {
  extern proc chpl_stopVerboseMemHere();
  chpl_stopVerboseMemHere();
}
