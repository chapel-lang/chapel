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

Complex is a predefined structured type in Chapel.

Like the integral and real types, the complex types can be parameterized
by the number of bits used to represent them. A complex number is
composed of two real numbers so the number of bits used to represent a
complex is twice the number of bits used to represent the real numbers.
The default complex type, ``complex``, is 128 bits; it consists of two
64-bit real numbers. The complex types that are supported are
machine-dependent, but usually include ``complex(64)`` and
``complex(128)``.

The real and imaginary components can be accessed via the methods ``re``
and ``im``. The type of these components is real. The standard :mod:`Math`
module provides some functions on complex types. See the :mod:`Math`
module documentation.

   *Example*.

   Given a complex number ``c`` with the value ``3.14+2.72i``, the
   expressions ``c.re`` and ``c.im`` refer to ``3.14`` and ``2.72``
   respectively.

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
    param bits;
    var re : real(bits);
    var im : real(bits);

    /*
      When used as a value, this returns the real component of
      the complex number as a *real*.

      When used as an lvalue, this is a setter that assigns the
      real component.
    */
    proc re ref {
      return 20.0;
    }

    /*
      When used as a value, this returns the imaginary component of
      the complex number as a *real*.

      When used as an lvalue, this is a setter that assigns the
      imaginary component.
    */
    proc im ref {
      return 20.0;
    }
  }
}
