/*
 * Copyright 2004-2015 Cray Inc.
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

// Wrapper for multithreaded FFTW version 3.

/*
  Multi-threaded FFT computations via FFTW (version 3)

  This module defines Chapel wrappers around key FFTW routines
  supporting multithreaded execution.  It builds directly on the
  :mod:`FFTW` module, inheriting all of its functionality, so refer to
  that module for documentation on the transforms themselves.

  To use this module:

  1. Ensure that FFTW (version 3) is installed on your system with
     multi-threaded support enabled and that the header and library
     files (e.g., fftw3.h, libfftw3.*) are either installed in a
     standard system location or that your C compiler's environment
     variables are set up to find them (alternatively, the Chapel
     compiler's ``-I`` and ``-L`` flags can be used to specify these
     locations).

  2. Add ``use FFTW_MT;`` to your Chapel code.

  3. Compile and run your Chapel program as usual.


  The steps to making multi-threaded FFTW calls are:

  0. By default, the module will initialize itself to use
     ``here.maxTaskPar`` threads per locale when calling
     :proc:`plan_dft* <FFTW.plan_dft>` routines.  This
     auto-initialization can be disabled by setting
     :param:`autoInitFFTW_MT` to `false` at compile-time and calling
     :proc:`init_FFTW_MT` and :proc:`plan_with_nthreads` manually.  At
     any time during program execution, the number of threads to be
     used by FFTW can be changed by calling :proc:`plan_with_nthreads`
     with a new value.
  
  1. Create, ...

  2. execute, ...

  3. ...and destroy plans as in single-threaded uses of :mod:`FFTW`.

  4. Call :proc:`cleanup_threads` to release the memory used by FFTW's
     threads and :proc:`~FFTW.cleanup` as in single-threaded uses of
     :mod:`FFTW`.

  Note that where the main :mod:`FFTW` module is a single-locale
  module, this one is multi-locale in that, once it is initialized,
  any locale can create and execute plans, though those plans will
  only be valid on that locale.  Calls to :proc:`init_FFTW_MT`,
  :proc:`plan_with_nthreads` and :proc:`cleanup_threads` will take
  those actions across all locales.
*/

//
// TODO: Should this module be a sub-module of FFTW?  I'm thinking
// so, but unfortunately this didn't occur to me in time for
// version 1.11's code freeze.  The main thing to worry about is
// whether 'use's of external dependences will be too aggressively
// resolved such that the MT library is linked in whenever vanilla
// FFTW is 'use'd.  That seems like it would be easy to fix, though.
//

module FFTW_MT {
  use FFTW;
  use "-lfftw3_threads", "-lpthread";

  /*
    By default, this module will call :proc:`init_FFTW_MT()` and
    :proc:`plan_with_nthreads()` as part of its initialization.
    Setting this to `false` at compile-time will disable this
    auto-initialization, requiring these calls to be made
    manually.
  */
  config param autoInitFFTW_MT = true;

  if autoInitFFTW_MT {
    //
    // If we're auto-initializing, when this module is used, it
    // calls init_FFTW_MT() and plan_with_nthreads().
    //
    init_FFTW_MT();
    plan_with_nthreads();
  }


  //
  // TODO: If users don't like the "on all locales" property, we
  // could consider making this a method on locale later to permit
  // either calling here.init_FFTW_MT or Locales.init_FFTW_MT...
  //
  // TODO: Incorporate a better error handling story
  //
  /*
    Initialize the :mod:`FFTW_MT` module.  This has the effect of
    calling the FFTW C routine ``fftw_init_threads()`` on all locales,
    halting the Chapel program if any of the calls generate an error.
  */
  proc init_FFTW_MT() {
    coforall loc in Locales {
      on loc do {
        if (C_FFTW.fftw_init_threads() == 0) then
          halt("Failed to properly initialize FFTW threads on locale ", 
               here.id);
      }
    }
  }


  //
  // TODO: As with the previous routine, we could consider making
  // this a method on locales if users want.
  //
  /*
    Register the number of threads to use for multi-threaded FFTW
    plans on all locales.  If fewer than one thread is requested, each
    locale will default to ``here.maxTaskPar`` threads.  Note that
    this routine can be called multiple times, overwriting previous
    values.

    :arg nthreads: The number of threads to use.
    :type nthreads: int
  */
  proc plan_with_nthreads(nthreads: int = 0) {
    coforall loc in Locales {
      on loc do {
        const myNThreads = if nthreads < 1 then here.maxTaskPar else nthreads;
        C_FFTW.fftw_plan_with_nthreads(myNThreads.safeCast(c_int));
      }
    }
  }


  /*
    Clean up the memory used by FFTW threads on all locales.
  */
  proc cleanup_threads() {
    coforall loc in Locales {
      on loc do {
        C_FFTW.fftw_cleanup_threads();
      }
    }
  }
}
