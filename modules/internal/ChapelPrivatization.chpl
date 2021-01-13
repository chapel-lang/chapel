/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

module ChapelPrivatization {

  private use CPtr;

  // the type of elements in chpl_privateObjects.
  pragma "no doc"
  extern record chpl_privateObject_t {
    var obj:c_void_ptr;
  }

  pragma "no doc"
  extern var chpl_privateObjects:c_ptr(chpl_privateObject_t);

  pragma "no doc"
  pragma "fn returns infinite lifetime"
  // should this use pragma "local args"?
  // Why is the compiler making the objectType argument wide?
  inline
  proc chpl_getPrivatizedCopy(type objectType, objectPid:int): objectType {
    return __primitive("cast", objectType, chpl_privateObjects[objectPid].obj);
  }

}
