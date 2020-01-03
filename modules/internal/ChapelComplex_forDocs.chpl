/*
 * Copyright 2004-2020 Cray Inc.
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
// NOAKES 2015/09/22
//
// This is a dummy chapel that is used to anchor some internal definitions
// during the last day before Doc Freeze for 1.12.  This file is processed
// for docs in modules/Makefile but is not part of the Chapel Module code
// base.
//
// One reason to create this file is that the Chapel compiler, and hence
// chpldoc, fails if it is asked to compile ChapelBase directly.  It is
// speculated that this is because ChapelBase auto-magically included as
// anchor module by the compiler.
//

//
// ChapelComplex_forDocs.chpl
//

/*

Complex is a predefined structured type in Chapel. It is specified
in the Types chapter of the Chapel Language Specification.

*/


module ChapelComplex_forDocs {

  /*
    It is defined as if it were a record with two fields.
    Note that both of these fields are of type *real*.
    Specifically the imaginary component is not of type *imag*.
    This is important when using the getter/setter for the
    field *im*.
  */

  record _complex {
    var re : real;
    var im : real;
  }

  /*
    When used as a value, this returns the real component of
    the complex number as a *real*.

    When used as an lvalue, this is a setter that assigns the
    real component.
  */
  proc complex.re ref {
    return 20.0;
  }

  /*
    When used as a value, this returns the imaginary component of
    the complex number as a *real*.

    When used as an lvalue, this is a setter that assigns the
    imaginary component.
  */
  proc complex.im ref {
    return 20.0;
  }
}
