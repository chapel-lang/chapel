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

  class _remoteVarContainer {
    type valueType;
    var containedValue: valueType;
  }

  record _remoteVarWrapper {
    type eltType;
    var tmp: owned _remoteVarContainer(eltType);

    proc ref get() ref {
      return tmp.containedValue;
    }
  }

  proc chpl__buildRemoteWrapper(loc: locale, type inType) {
    var default: inType;
    return chpl__buildRemoteWrapper(loc, inType, default);
  }

  proc chpl__buildRemoteWrapper(loc: locale, in value: ?t) {
    return chpl__buildRemoteWrapper(loc, t, value);
  }

  proc chpl__buildRemoteWrapper(loc: locale, type inType, in value: inType) {
    var c: owned _remoteVarContainer(inType)?;
    on loc do c = new _remoteVarContainer(inType, value);
    return new _remoteVarWrapper(inType, try! c : owned _remoteVarContainer(inType));
  }

}
