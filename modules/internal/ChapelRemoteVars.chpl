/*
 * Copyright 2024 Hewlett Packard Enterprise Development LP
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

module ChapelRemoteVars {
  use OwnedObject;
  use ChapelIteratorSupport;

  class _remoteVarContainer {
    var containedValue;
  }

  record _remoteVarWrapper {
    type eltType;
    var tmp: owned _remoteVarContainer(eltType);

    proc init(in tmp: owned _remoteVarContainer(?eltType)) {
      this.eltType = eltType;
      this.tmp = tmp;
    }

    proc ref get() ref {
      return tmp.containedValue;
    }
  }

  inline proc __defaultValueForType(type inType) {
    var default: inType;
    return default;
  }

  @unstable("remote variables are unstable")
  inline proc chpl__buildRemoteWrapper(loc: locale, type inType) {
    return chpl__buildRemoteWrapper(loc, inType, __primitive("create thunk", __defaultValueForType(inType)));
  }

  @unstable("remote variables are unstable")
  inline proc chpl__buildRemoteWrapper(loc: locale, in tr: _thunkRecord) {
    return chpl__buildRemoteWrapper(loc, thunkToReturnType(tr.type), tr);
  }

  @unstable("remote variables are unstable")
  inline proc chpl__buildRemoteWrapper(loc: locale, type inType, in tr: _thunkRecord) {
    var c: owned _remoteVarContainer(inType)?;
    on loc do c = new _remoteVarContainer(__primitive("force thunk", tr));
    return new _remoteVarWrapper(try! c : owned _remoteVarContainer(inType));
  }

}
