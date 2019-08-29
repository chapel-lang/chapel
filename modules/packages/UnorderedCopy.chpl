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

/*
   .. warning::
     This module represents work in progress. The API is unstable and likely to
     change over time.

   This module provides an unordered version of copy/assign for ``numeric``
   types. The results from this function are not visible until task or forall
   termination or an explicit :proc:`unorderedCopyTaskFence()`, but it can
   provide a significant speedup for bulk assignment operations that do not
   require ordering of operations:

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
     required for best performance. In our experience, buffered gets can
     achieve up to a 5X performance improvement over non-buffered gets for CLE
     5.2UP04 or newer.
 */
module UnorderedCopy {
  /*
     Unordered copy. Only supported for identical numeric and bool types.
   */
  inline proc unorderedCopy(ref dst:numeric, const ref src:numeric): void {
    unorderedCopyPrim(dst, src);
  }

  pragma "no doc"
  inline proc unorderedCopy(ref dst:numeric, param src:numeric): void {
    const refSrc = src;
    unorderedCopyPrim(dst, refSrc);
  }

  inline proc unorderedCopy(ref dst:bool(?), const ref src:bool(?)): void {
    unorderedCopyPrim(dst, src);
  }

  pragma "no doc"
  inline proc unorderedCopy(ref dst:bool(?), param src:bool(?)): void {
    const refSrc = src;
    unorderedCopyPrim(dst, refSrc);
  }

  private inline proc unorderedCopyPrim(ref dst, const ref src): void {
    param sameType = dst.type == src.type;
    param validType = isNumeric(dst) || isBool(dst);
    if !sameType || !validType then
      compilerError("unorderedCopy is only supported between identical numeric and bool types");

    if CHPL_COMM == 'ugni' {
      __primitive("unordered=", dst, src);
    } else {
      __primitive("=", dst, src);
    }
  }

  /*
     Fence any pending unordered copies issued by the current task.
   */
  inline proc unorderedCopyTaskFence(): void {
    if CHPL_COMM == 'ugni' {
      extern proc chpl_comm_getput_unordered_task_fence();
      chpl_comm_getput_unordered_task_fence();
    }
  }

  /*
   .. warning::
     This function has been deprecated - please use
     :proc:`unorderedCopyTaskFence()` instead. Note that this function has
     been deprecated without a full release of support because the previous
     global fence semantics imposed expensive implementation requirements and
     is not expected to be needed now that operations are implicitly fenced at
     task/forall termination.
  */
  inline proc unorderedCopyFence(): void {
    compilerError("unorderedCopyFence() is no longer supported - please use unorderedCopyTaskFence() instead");
    unorderedCopyTaskFence();
  }
}
