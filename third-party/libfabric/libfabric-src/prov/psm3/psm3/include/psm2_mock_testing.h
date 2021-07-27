/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2016 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef PSM2_MOCK_TESTING_H
#define PSM2_MOCK_TESTING_H

/* PSM2_MOCK_TESTING being defined flips a couple of switches so that a
 * testable version of libpsm2.so is built. It'll make properly annotated
 * static functions be non-static, visible to the outside. Also, all mockable
 * functions will be replaced with function pointers which will originally
 * point to the actual implementation. However, those function pointers might
 * be reset by the test code, thus allowing for mocking selected PSM2 functions
 * for the purpose of the test.
 *
 * So far the following utilities have been introduced for enabling a
 * conditional compilation of the testable vs. production version of the library:
 *  - ustatic: toggles function visibility
 *  - MOCKABLE(): decorates function name so that it is visible after being mocked
 *  - MOCK_DCL_EPILOGUE(): declares a function pointer which will be the seam
 *        for mocking a function
 *  - MOCK_DEF_EPILOGUE(): defines a function pointer which will be the seam
 *        for mocking a function
 *
 * If the declaration and definition of a static function @c foo reside in
 * different files, this would be the common use case:
 *
 * @code
 * // somefile.c:
 * int MOCKABLE(foo)();
 * MOCK_DCL_EPILOGUE(foo);
 *
 * // otherfile.c:
 * int MOCKABLE(foo)() {
 * 	printf("I am the original foo!\n");
 * }
 * MOCK_DEF_EPILOGUE(foo);
 * @endcode
 *
 * If the production version of the library is being built, the following code
 * would result:
 * @code
 * // somefile.c:
 * int foo();
 *
 * // otherfile.c:
 * int foo() {
 * 	printf("I am the original foo!\n");
 * }
 * @endcode
 *
 * On the other hand, if a testable version of the libary is being build, it
 * would produce the following code:
 * @code
 * // somefile.c:
 * int foo_original_();
 * extern typeof(& foo_original_) foo;
 *
 * // otherfile.c:
 * int foo_original_() {
 * 	printf("I am the original foo!\n");
 * }
 * typeof(& foo_original_) foo = foo_original_;
 * @endcode
 *
 * If the function to be mocked is a static function residing in the header,
 * the following syntax would be used:
 * @code
 * // somefile.c:
 * ustatic int MOCKABLE(foo)() {
 * 	printf("I am the original foo!\n");
 * }
 * MOCK_DCL_EPILOGUE(foo);
 * MOCK_DEF_EPILOGUE(foo);
 * @endcode
 *
 * If the production version of the library is being built, the following code
 * would result:
 * @code
 * // somefile.c:
 * static int foo() {
 * 	printf("I am the original foo!\n");
 * }
 * @endcode
 *
 * Similarly, if a testable version of the libary is being build, it would
 * produce the following code:
 * @code
 * // somefile.c:
 * int foo_original_();
 * extern typeof(& foo_original_) foo;
 * typeof(& foo_original_) foo = foo_original_;
 * @endcode
 */
#ifndef PSM2_MOCK_TESTING

/* If no testing is being done, ustatic resolves to regular "static" */
#define ustatic static
/* If no testing is being done, no indirection is introduced */
#define MOCKABLE(fname) fname
/* If no testing is being done, no declaration epilogue is needed */
#define MOCK_DCL_EPILOGUE(fname)
/* If no testing is being done, no definition epilogue is needed */
#define MOCK_DEF_EPILOGUE(fname)

#else /* ndef PSM2_MOCK_TESTING */

/* For the testable version, all _ustatic_ function will NOT be static */
#define ustatic
/* TODO override inline directives in the same fashion as static */
/* For the testable version, the actual implementation function is renamed */
#define MOCKABLE(x) x ## _original_
/* For the testable version, we declare the function pointer which will be the
 * point of indirection for calls to that function. It must be delared after
 * the declaration of the actual function happens.
 */
#define MOCK_DCL_EPILOGUE(x) extern typeof(& x ## _original_) x;
/* For the testable version, we define the function pointer which will be the
 * point of indirection for calls to that function. It must be delared after
 * the definition of the actual function happens.
 */
#define MOCK_DEF_EPILOGUE(x) typeof(& x ## _original_) x = x ## _original_;

#endif /* ndef PSM2_MOCK_TESTING */

#endif /* PSM2_MOCK_TESTING_H */

