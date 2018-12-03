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

/*
   .. warning::
     This module represents work in progress. The API is unstable and likely to
     change over time.

   This module provides a buffered version of network gets for ``numeric``
   types. The get is internally buffered and the buffers are flushed
   implicitly when full or explicitly with :proc:`flushGetBuff()`. This
   buffered operation can provide a significant speedup for bulk GET operations
   that do not require strict ordering of operations.

   .. TODO performance example

   It's important to be aware that buffered GET operations are not consistent
   with regular operations and updates may not be visible until the buffers are
   explicitly flushed with :proc:`flushGetBuff()`.

   .. code-block:: chapel

     var a = 0;
     on Locales[1] {
       var b = 1;
       getBuff(b, a);
       writeln(b);        // can print 0 or 1
       flushGetBuff();
       writeln(b);        // must print 0
     }

   Generally speaking they are useful for when you have a large batch of GETs
   to perform and the order of those operations doesn't matter.

   .. note::
     Currently, this is only optimized for ``CHPL_COMM=ugni``. Other
     communication layers fall back to regular gets. Under ugni GETs
     are internally buffered. When the buffers are flushed, the operations are
     performed all at once. Cray Linux Environment (CLE) 5.2.UP04 or newer is
     required for best performance. In our experience, buffered gets can
     achieve up to a 5X performance improvement over non-buffered gets for
     CLE 5.2UP04 or newer.
 */
module BufferedGets {
  /*
     Buffered GET operation. Only supported for numeric types.
   */
  inline proc getBuff(ref LHS, ref RHS): void {
    if !(isNumericType(LHS.type) && LHS.type == RHS.type) then
      compilerError("getBuff is only supported on numeric types");
    if CHPL_COMM == 'ugni' {
      __primitive("chpl_comm_buff_get", LHS, RHS.locale.id, RHS, numBytes(RHS.type));
    } else {
      __primitive("chpl_comm_get", LHS, RHS.locale.id, RHS, numBytes(RHS.type));
    }
  }

  /*
     Flush any get operations that are still buffered. Note that this
     flushes any pending operations on all locales, not just the current
     locale.
   */
  inline proc flushGetBuff(): void {
    if CHPL_COMM == 'ugni' {
      extern proc chpl_comm_get_buff_flush();
      coforall loc in Locales do on loc {
        chpl_comm_get_buff_flush();
      }
    }
  }

}
