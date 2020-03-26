/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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
  Matrix and Vector Norms

  The Norm module currently defines:

    * 1-norm, 2-norm, infinity norm and Frobenius norm for 1D arrays
    * 1-norm, infinity norm and Frobenius norm for 2D arrays

  The :enum:`normType` argument for the first few overloads of :proc:`!norm` is
  an enumeration indicating the norm to compute.

  The norm function may also be called without specifying the :enum:`normType`, in
  which case the default norm for that array rank will be used.
 */
module Norm {

/*
  Indicates the different types of norms supported by this module:

    * 1-norm
    * 2-norm
    * Infinity norm
    * Frobenius norm
 */
enum normType {
  norm1,
  norm2,
  normInf,
  normFrob
};

/*
  Compute the norm indicated by `p` on the 1D array `x`.

  :rtype: x.eltType
 */
proc norm(x: [], p: normType) where x.rank == 1 {
  select (p) {
  when normType.norm1 do return + reduce abs(x);
  when normType.norm2 do return sqrt(+ reduce (abs(x)*abs(x)));
  when normType.normInf do return max reduce abs(x);
  when normType.normFrob do return sqrt(+ reduce (abs(x)*abs(x)));
  otherwise halt("Unexpected norm type");
  }
}

/*
  Compute the norm indicated by `p` on the 2D array `x`.

  `p` cannot be `normType.norm2`.

  :rtype: x.eltType
 */
proc norm(x: [?D], p: normType) where x.rank == 2 {
  select (p) {
  when normType.norm1 do
    return max reduce forall j in D.dim(2) do (+ reduce abs(x[D.dim(1), j..j]));
  
  when normType.norm2 do
    halt("2-norm for 2D arrays are not yet implemented");
  
  when normType.normInf do
    return max reduce forall i in D.dim(1) do (+ reduce abs(x[i..i, D.dim(2)]));
  
  when normType.normFrob do return sqrt(+ reduce (abs(x)*abs(x)));
  
  otherwise halt("Unexpected norm type");
  }
}

// this module doesn't implement norms for > 2D arrays, so generate
// a compile-time error if the user tries to call one
pragma "no doc"
proc norm(x: [], p: normType) where x.rank > 2 {
  compilerError("Norms not implemented for array ranks > 2D");
}

/*
  Compute the default norm on `x`.

  For a 1D array this is the 2-norm, for a 2D array, this is the Frobenius
  norm.

  :rtype: x.eltType
 */
proc norm(x: []) {
  select (x.rank) {
  when 1 do return norm(x, normType.norm2);
  when 2 do return norm(x, normType.normFrob);
  otherwise compilerError("Norms not implemented for array ranks > 2D");
  }
}

} // end module Norm
