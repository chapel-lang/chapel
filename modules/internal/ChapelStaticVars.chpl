/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

module ChapelStaticVars {
  use OwnedObject;
  use Atomics;
  use ChapelLocale;
  use LocalesArray;

  pragma "sharing kind enum"
  enum sharingKind {
    /**
      Default distribution mode. The value is computed on whichever
      locale first calls the function; other locales remotely access
      the value once it's computed.
     */
    computeOrRetrieve,
    /**
      The variable is computed per-locale; the first call to the function
      on each locale causes it to compute the static variable.
     */
    computePerLocale,
  }

  class _staticWrapperContainer {
    var value;
  }

  record _staticWrapper {
    type valueType;
    var container: owned _staticWrapperContainer(valueType)?;

    // Values of inited:
    //   0, initial state -- needs a value.
    //   1, a caller is computing the required value.
    //   2, the value has been computed.

    var inited: atomic int;

    proc init(type valueType) {
      this.valueType = valueType;
      this.container = nil;
      this.inited = 0;
    }

    inline proc ref setValue(in v: valueType) {
      this.container = new _staticWrapperContainer(v);
      this.inited.write(2);
    }

    inline proc ref getValue() ref : valueType {
      this.inited.waitFor(2);
      return this.container!.value;
    }

    inline proc ref callerShouldComputeValue() do {
      if this.inited.read() == 2 then return false;
      var expected = 0;
      return this.inited.compareExchange(expected, 1);
    }

    proc ref reset() do {
      this.container = nil;
    }
  }

  proc chpl__functionStaticVariableWrapperType(type valueType) type
    do return _staticWrapper(valueType);

  proc chpl__executeStaticWrapperCleanupEverywhere(fn: proc(): void) {
    for loc in Locales do on loc do fn();
  }

}
