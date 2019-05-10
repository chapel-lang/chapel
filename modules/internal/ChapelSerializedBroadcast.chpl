/*
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
    //
    // BLC: I can't explain the need for the following conditional, but if I
    // don't place it here, when the compiler tries to resolve this
    // call for a Block-distributed array as part of resolveBroadcasters(), 
    // the 'data' value below ends up being a 2*int(64) which causes an
    // error when trying to resolve the deserialize() routine since it
    // only expects an int(64) for Block-distributed arrays.  But it's
    // also a bit weird that it generates an error at all given that
    // (a) resolveBroadcasters() ought to be doing speculative resolution
    // that doesn't generate problems for the user if it fails (I think?)
    // and (b) the chpl__serialize() overload on the _array type ought to
    // have a 'false' where clause for Block-distributed arrays.
    // That said, bypassing arrays in this case ought to be OK because
    // I don't think we should be broadcasting global arrays anyway?
    // (and definitely not block-distributed ones).  I put the halt()
    // in here out of a sense of wanting to know if/when we actually
    // try to use this code path.
    //
    if isArray(localeZeroGlobal) {
      halt("internal error: can't broadcast module-scope arrays yet");
    } else {
      const data = localeZeroGlobal.chpl__serialize();
      const root = here.id;
      coforall loc in Locales do on loc {
        if here.id != root {
          pragma "no copy"
          pragma "no auto destroy"
          var temp = localeZeroGlobal.type.chpl__deserialize(data);

          const destVoidPtr = chpl_get_global_serialize_table(id);
          const dest = destVoidPtr:c_ptr(localeZeroGlobal.type);

          __primitive("=", dest.deref(), temp);
        }
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
