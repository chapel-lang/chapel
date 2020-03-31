.. default-domain:: chpl

.. module:: FFTW

FFTW
====
**Usage**

.. code-block:: chapel

   use FFTW;

**Submodules**

.. toctree::
   :maxdepth: 1
   :glob:

   FFTW/*

.. data:: config param isFFTW_MKL = false

   
   Set this to `true` if you are using the Intel MKL FFTW
   wrappers
   

.. data:: config param autoInitFFTW_MT = false

   
   Set this config parameter to `true` to automatically initialize
   FFTW for thread support, and setup FFTW to generate multi-threaded
   plans (with the number of threads equal to `maxTaskPar` in Chapel).
   
   If you keep the default value of `false`, then call :proc:`init_FFTW_MT()`
   to initialize thread-support for Chapel.
   
   Note that :proc:`plan_with_nthreads` can be called at any time
   and changes the number of threads used by plans created after the call.
   

.. data:: config param noFFTWsizeChecks = false

   
   Controls execution-time array size checks in the FFTW
   :proc:`plan_dft` routines (set to `true` to disable checks).
   

.. type:: type fftw_plan

   
   An opaque type used to store and reuse FFTW plans across multiple
   routines.
   

.. type:: type FFTW_Flag = c_uint

   
   Type alias for FFTW flags
   

.. type:: type FFTW_Direction = c_int

   
   Type alias for FFT directions
   

.. type:: type FFTW_R2R = c_int

   
   Type alias for FFTW R2R type
   

.. function:: proc plan_dft(input: [?Din] complex(128), output: [?Dout] complex(128), sign: FFTW_Direction, flags: FFTW_Flag): fftw_plan

   
   Creates a plan for an out-of-place complex-to-complex DFT.
   
   :arg input: The input array, which can be of any rank
   :type input: [] `complex(128)`
   
   :arg output: The output array, whose size and shape must match the input array's
   :type output: [] `complex(128)`
   
   :arg sign: :const:`FFTW_FORWARD` or :const:`FFTW_BACKWARD`
   :type sign: `FFTW_Direction`
   
   :arg flags: the bitwise-or of any planning-rigor or algorithm-restriction flags that should be used in creating the plan (e.g., :const:`FFTW_MEASURE` ``|`` :const:`FFTW_PRESERVE_INPUT`)
   :type flags: `FFTW_Flag`
   
   :returns: The :type:`fftw_plan` representing the resulting plan
   

.. function:: proc plan_dft(arr: [] complex(128), sign: FFTW_Direction, flags: FFTW_Flag): fftw_plan

   
   Creates a plan for an in-place complex-to-complex DFT.
   
   :arg arr: The array to use as the in-place input/output array.
   :type arr: [] `complex(128)`
   
   :arg sign: :const:`FFTW_FORWARD` or :const:`FFTW_BACKWARD`
   :type sign: `FFTW_Direction`
   
   :arg flags: the bitwise-or of any planning-rigor or algorithm-restriction flags that should be used in creating the plan (e.g., :const:`FFTW_MEASURE` ``|`` :const:`FFTW_PRESERVE_INPUT`)
   :type flags: `FFTW_Flag`
   
   :returns: The :type:`fftw_plan` representing the resulting plan
   

.. function:: proc plan_dft_r2c(input: [?Din] real(64), output: [?Dout] complex(128), flags: FFTW_Flag): fftw_plan

   
   Create a plan for a real-to-complex, out-of-place DFT.
   
   :arg input: The input array, which can be of any rank
   :type input: [] `real(64)`
   
   :arg output: The output array, whose size and shape must match the input array's, except for the leading dimension which should be n/2 + 1, where n is the size of the input array's leading dimension.  See the `FFTW documentation <http://www.fftw.org/fftw3_doc/Multi_002dDimensional-DFTs-of-Real-Data.html#Multi_002dDimensional-DFTs-of-Real-Data>`_ for more information.
   :type output: [] `complex(128)`
   
   :arg flags: the bitwise-or of any planning-rigor or algorithm-restriction flags that should be used in creating the plan (e.g., :const:`FFTW_MEASURE` ``|`` :const:`FFTW_PRESERVE_INPUT`)
   :type flags: `FFTW_Flag`
   
   :returns: The :type:`fftw_plan` representing the resulting plan
   

.. function:: proc plan_dft_r2c(realDom: domain, arr: [?D] ?t, flags: FFTW_Flag): fftw_plan

   
   Create a plan for a real-to-complex, in-place DFT.
   
   :arg realDom: Describes the indices of the 'real' view of the array
   :type realDom: `domain`
   
   :arg arr: The array to be used as the in-place input/output array.  If passing in an array of `real` elements, the leading dimension of the array must be padded to store 2(n/2 + 1) elements, where `n` is the size of the corresponding dimension of `realDom`.  If passing in an array of `complex` elements, the leading dimension should be (n/2 + 1).  See the `FFTW documentation <http://www.fftw.org/fftw3_doc/Multi_002dDimensional-DFTs-of-Real-Data.html#Multi_002dDimensional-DFTs-of-Real-Data>`_ for more information.
   :type arr: [] `T` where `T` is of type `real(64)` or `complex(128)`
   
   :arg flags: the bitwise-or of any planning-rigor or algorithm-restriction flags that should be used in creating the plan (e.g., :const:`FFTW_MEASURE` ``|`` :const:`FFTW_PRESERVE_INPUT`)
   :type flags: `FFTW_Flag`
   
   :returns: The :type:`fftw_plan` representing the resulting plan
   

.. function:: proc plan_dft_c2r(input: [?Din] complex(128), output: [?Dout] real(64), flags: FFTW_Flag): fftw_plan

   
   Create a plan for a complex-to-real, out-of-place DFT.
   
   :arg input: The input array, whose size and shape must match the output array's, except for the leading dimension which should be n/2 + 1, where n is the size of the output array's leading dimension.  See the `FFTW documentation <http://www.fftw.org/fftw3_doc/Multi_002dDimensional-DFTs-of-Real-Data.html#Multi_002dDimensional-DFTs-of-Real-Data>`_ for more information.
   :type input: [] `complex(128)`
   
   :arg output: The output array
   :type output: [] `real(64)`
   
   :arg flags: the bitwise-or of any planning-rigor or algorithm-restriction flags that should be used in creating the plan (e.g., :const:`FFTW_MEASURE` ``|`` :const:`FFTW_PRESERVE_INPUT`)
   :type flags: `FFTW_Flag`
   
   :returns: The :type:`fftw_plan` representing the resulting plan
   

.. function:: proc plan_dft_c2r(realDom: domain, arr: [?D] ?t, flags: FFTW_Flag): fftw_plan

   
   Create a plan for a complex-to-real, in-place DFT.
   
   :arg realDom: Describes the indices of the 'real' view of the array
   :type realDom: `domain`
   
   :arg arr: The array to be used as the in-place input/output array.  If passing in an array of `real` elements, the leading dimension of the array must be padded to store 2(n/2 + 1) elements, where `n` is the size of the corresponding dimension of `realDom`.  If passing in an array of `complex` elements, the leading dimension should be (n/2 + 1).  See the `FFTW documentation <http://www.fftw.org/fftw3_doc/Multi_002dDimensional-DFTs-of-Real-Data.html#Multi_002dDimensional-DFTs-of-Real-Data>`_ for more information.
   :type arr: [] `T` where `T` is of type `real(64)` or `complex(128)`
   
   :arg flags: the bitwise-or of any planning-rigor or algorithm-restriction flags that should be used in creating the plan (e.g., :const:`FFTW_MEASURE` ``|`` :const:`FFTW_PRESERVE_INPUT`)
   :type flags: `FFTW_Flag`
   
   :returns: The :type:`fftw_plan` representing the resulting plan
   

.. function:: proc execute(const plan: fftw_plan)

   
   Execute an FFTW plan.
   
   :arg plan: The plan to execute, as computed by a `plan_dft*()` routine.
   :type plan: `fftw_plan`
   

.. function:: proc destroy_plan(plan: fftw_plan)

   
   Destroy an FFTW plan.
   
   :arg plan: The plan to destroy
   :type plan: `fftw_plan`
   

.. function:: proc cleanup()

   
   Clean up FFTW overall.
   

.. data:: const FFTW_FORWARD: FFTW_Direction

   Request a forward transform (i.e., use a negative exponent in the transform). 

.. data:: const FFTW_BACKWARD: FFTW_Direction

   Request a backward transform (i.e., use a positive exponent in the transform). 

.. data:: const FFTW_ESTIMATE: FFTW_Flag

   Specify that a simple heuristic should be used to pick a plan
   quickly.  This will prevent the input/output arrays from being
   overwritten during planning. 

.. data:: const FFTW_MEASURE: FFTW_Flag

.. data:: const FFTW_PATIENT: FFTW_Flag

   Specify that FFTW should expend a greater effort finding an
   optimized plan. 

.. data:: const FFTW_EXHAUSTIVE: FFTW_Flag

   Specify that FFTW should expend an even greater effort finding an
   optimized plan. 

.. data:: const FFTW_WISDOM_ONLY: FFTW_Flag

   This is a special planning mode that is useful for querying
   whether wisdom is available.  When using it, the plan is only
   created when wisdom is available for the given problem; otherwise
   a `null plan` is returned.  This can be combined with other flags
   to create a plan if the wisdom available was created in that mode
   (e.g., :const:`FFTW_WISDOM_ONLY` ``|`` :const:`FFTW_PATIENT`).  For more details on
   this flag and the previous four, refer to `Section 4.3.2
   <http://www.fftw.org/doc/Planner-Flags.html>`_ of the FFTW manual
   

.. data:: const FFTW_DESTROY_INPUT: FFTW_Flag

.. data:: const FFTW_PRESERVE_INPUT: FFTW_Flag

.. data:: const FFTW_UNALIGNED: FFTW_Flag

.. data:: const FFTW_R2HC: FFTW_R2R

   Use the halfcomplex form of array storage 

.. data:: const FFTW_HC2R: FFTW_R2R

.. data:: const FFTW_DHT: FFTW_R2R

   Discrete Hartley Transforms. 

.. data:: const FFTW_REDFT00: FFTW_R2R

   Specify the type of discrete cosine and
   discrete sine transforms to use. 

.. data:: const FFTW_REDFT01: FFTW_R2R

.. data:: const FFTW_REDFT10: FFTW_R2R

.. data:: const FFTW_REDFT11: FFTW_R2R

.. data:: const FFTW_RODFT00: FFTW_R2R

.. data:: const FFTW_RODFT01: FFTW_R2R

.. data:: const FFTW_RODFT10: FFTW_R2R

.. data:: const FFTW_RODFT11: FFTW_R2R

.. function:: proc init_FFTW_MT()

   
   Initialize the `FFTW` module to support multithreading.  This has the
   effect of calling the FFTW C routine ``fftw_init_threads()`` on all
   locales, halting the Chapel program if any of the calls generate an error.
   

.. function:: proc plan_with_nthreads(nthreads: int = 0)

   
   Register the number of threads to use for multi-threaded FFTW
   plans on all locales.  If fewer than one thread is requested, each
   locale will default to ``here.maxTaskPar`` threads.  Note that
   this routine can be called multiple times, overwriting previous
   values.
   
   :arg nthreads: The number of threads to use.
   :type nthreads: `int`
   

.. function:: proc cleanup_threads()

   
   Clean up the memory used by FFTW threads on all locales.
   

