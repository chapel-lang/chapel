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

//
// Used by the compiler to deeply broadcast global consts
//
module ChapelSerializedBroadcast {
  use ChapelLocale;
  use CPtr;

  config param chpl__enableSerializedGlobals = true;

  extern proc chpl_get_global_serialize_table(idx : int) : c_void_ptr;

  proc chpl__broadcastGlobal(ref localeZeroGlobal : ?T, id : int)
  where chpl__enableSerializedGlobals {
    const data = localeZeroGlobal.chpl__serialize();
    const root = here.id;
    coforall loc in Locales do on loc {
      if here.id != root {
        pragma "no copy"
        pragma "no auto destroy"
        var temp = T.chpl__deserialize(data);

        const destVoidPtr = chpl_get_global_serialize_table(id);
        const dest = destVoidPtr:c_ptr(localeZeroGlobal.type);

        __primitive("=", dest.deref(), temp);
      }
    }
  }

  proc chpl__destroyBroadcastedGlobal(ref localeZeroGlobal, id : int)
  where chpl__enableSerializedGlobals {
    type globalType = localeZeroGlobal.type;
    const root = here.id;
    coforall loc in Locales do on loc {
      if here.id != root {
        const voidPtr = chpl_get_global_serialize_table(id);
        var ptr = voidPtr:c_ptr(globalType);

        pragma "no copy"
        pragma "no auto destroy"
        var temp = ptr.deref();

        chpl__autoDestroy(temp);
      }
    }
  }
}
