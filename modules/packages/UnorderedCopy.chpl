/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

   This module provides an unordered version of copy/assign for trivially
   copyable types. Trivially copyable types require no special behavior to be
   copied and merely copying their representation is sufficient. They include
   ``numeric`` and ``bool`` types as well as tuples or records consisting only
   of ``numeric``/``bool``. Records cannot have user-defined copy-initializers,
   deinitializers, or assignment overloads.

   :proc:`unorderedCopy()` can provide a significant speedup for batch
   assignment operations that do not require ordering of operations. The
   results from :proc:`unorderedCopy()` are not visible until task or forall
   termination or an explicit :proc:`unorderedCopyTaskFence()`:

   .. code-block:: chapel

     use BlockDist, UnorderedCopy;

     const size = 10000;
     const space = {0..size};
     const D = space dmapped Block(space);
     var A, reversedA: [D] int = D;

     forall i in D do
       unorderedCopy(reversedA[i], A[size-i]);

     // no fence required, fenced at task/forall termination

     forall (rA, i) in zip(reversedA, D) do
       assert(rA == size-i);


   It's important to be aware that unordered operations are not consistent with
   regular operations and updates may not be visible until the task or forall
   that issued them terminates or an explicit :proc:`unorderedCopyTaskFence()`.

   .. code-block:: chapel

     var a = 0;
     on Locales[1] {
       var b = 1;
       unorderedCopy(b, a);
       writeln(b);        // can print 0 or 1
       unorderedCopyTaskFence();
       writeln(b);        // must print 0
     }


   Generally speaking they are useful for when you have a large batch of remote
   assignments to perform and the order of those operations doesn't matter.

   .. note::
     Currently, this is only optimized for ``CHPL_COMM=ugni``. Other
     communication layers fall back to regular operations. Under ugni, GETs are
     internally buffered. When the buffers are flushed, the operations are
     performed all at once. Cray Linux Environment (CLE) 5.2.UP04 or newer is
     required for best performance. In our experience, unordered copies can
     achieve up to a 5X performance improvement over ordered copies for CLE
     5.2UP04 or newer.
 */
module UnorderedCopy {
  /*
     Unordered copy. Only supported for identical trivially copyable types.
   */
  // Version to provide a clean signature for docs and to provide a clean error
  // message instead of just "unresolved call". Last resort to avoid thwarting
  // promotion for POD arrays.
  pragma "last resort"
  inline proc unorderedCopy(ref dst, src): void {
    compilerError("unorderedCopy is only supported between identical trivially copyable types");
  }

  pragma "no doc"
  inline proc unorderedCopy(ref dst:chpl_anyPOD, const ref src:chpl_anyPOD): void {
    unorderedCopyPrim(dst, src);
  }

  pragma "no doc"
  inline proc unorderedCopy(ref dst:chpl_anyPOD, param src:chpl_anyPOD): void {
    const refSrc = src;
    unorderedCopyPrim(dst, refSrc);
  }

  private inline proc unorderedCopyPrim(ref dst, const ref src): void {
    param sameType = dst.type == src.type;
    param validType = isPOD(dst);
    if !sameType || !validType then
      compilerError("unorderedCopy is only supported between identical trivially copyable types");

    __primitive("unordered=", dst, src);
  }

  /*
     Fence any pending unordered copies issued by the current task.
   */
  inline proc unorderedCopyTaskFence(): void {
    extern proc chpl_gen_comm_getput_unordered_task_fence();
    chpl_gen_comm_getput_unordered_task_fence();
  }
}
