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
  Support for key multithreaded FFTW version 3 routines in Chapel trappings

  This module defines a Chapel wrapper around key routines from the
  multithreaded support of FFTW version 3.  This module builds on the
  FFTW module itself and inherits all of its functionality, so refer there
  for most of the documentation.

  The main differences in using this module are:

  0) By default, the module will initialize itself to use
     here.maxTaskPar threads per locale when calling plan_dft*()
     routines.  To override this default or change it during program
     execution, call plan_with_nthreads() manually.
  
  1-3) As in FFTW

  3a) Call cleanup_threads() to release memory used by the FFTW threads.

  Note that whereas the main FFTW.chpl module is a single-locale
  module, this module is multi-locale in that, once it is loaded, any
  locale can create and execute plans, though those plans will only be
  valid on that locale.  Calls to plan_with_nthreads() and
  cleanup_threads() will make those actions occur on all locales.

  At present, clients of the FFTW module must specify fftw3.h,
  -lfftw3, -lfftw3_threads on the command line with the appropriate -I
  and -L flags sufficient to find the files.
*/

module FFTW_MT {
  use FFTW;

  /*
    By default, this module will call init_FFTW_MT() and
    plan_with_nthreads() as part of its initialization.  Users can
    diable this by setting autoInitFFTW_MT at compile-time, in which
    case, they will need to make the calls manually.
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
    Initialize the FFTW_MT module.  This has the effect of calling
    fftw_init_threads() on all locales.  This routine halts the Chapel
    program if any of the calls generate an error.
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
    Registers the number of threads to use for multi-threaded FFTW
    plans on all locales.  If fewer than one thread is requested, each
    locale will default to here.maxTaskPar threads.  Note that this
    routine can be called multiple times, overwriting previous values.

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
    Cleans up the threads if multi-threaded FFTW's were enabled using
    :proc:`init_threads()`.
  */
  proc cleanup_threads() {
    coforall loc in Locales {
      on loc do {
        C_FFTW.fftw_cleanup_threads();
      }
    }
  }
}
