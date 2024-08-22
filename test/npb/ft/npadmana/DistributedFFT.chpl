/*
    Provides a distributed 3D FFT.

    .. note::

     * Currently only supports complex-complex and real-real transforms
     * Only out-of-place transforms are supported, and the output array
       has its first two indices transposed. The input array is unchanged.
     * Requires FFTW for the local 1D transforms.
     * The data are slab-distributed along the first dimension.


    A simple example of using the module is

    .. code-block:: chapel

      // Set up a slab-distributed domain
      const Dom  = newSlabDom({0.. #NX, 0.. #NY, 0.. #NZ});
      // And for the transposed array
      const DomT = newSlabDom({0.. #NY, 0.. #NX, 0.. #NZ});

      // Define the array
      var inputarr  : [Dom]  complex;
      var outputarr : [DomT] complex;

      //Process inputarr as necessary
      //then FFT in the forward direction
      doFFT_Transposed(FFTtype.DFT, inputarr, outputarr, FFTW_FORWARD);

      //Process outputarr as necessary
      //then FFT in the reverse direction
      doFFT_Transposed(FFTtype.DFT, outputarr, inputarr, FFTW_BACKWARD);

*/
prototype module DistributedFFT {

  public use BlockDist;
  public use FFTW;
  import FFTW; // also allow FFTW.execute, etc

  use AllLocalesBarriers;
  use RangeChunk;
  use FFTW.C_FFTW;
  use FFT_Locks;
  use FFT_Timers;
  use CTypes;

  /*
    Compile time parameters for higher performance.

    ``usePerformant`` selects between a naive pencil-and-paper
    algorithm and a more performant version.
  */
  config param usePerformant=true;
  /*

    ``usePrimitiveComm`` calls into non-user-facing Chapel
    communication primitives for data movement, instead of using
    array slicing and copying.
  */
  config param usePrimitiveComm=true;

  @chpldoc.nodoc
  proc deinit() {
    cleanup();
  }

  /*
      The type of the transform.

      Current complex-complex (DFT) and
      real-to-real (R2R) transforms are supported.
  */
  enum FFTtype {DFT, R2R};

  /* fftw_plan fftw_plan_many_dft(int rank, const int *n, int howmany, */
  /*                              fftw_complex *in, const int *inembed, */
  /*                              int istride, int idist, */
  /*                              fftw_complex *out, const int *onembed, */
  /*                              int ostride, int odist, */
  /*                              int sign, unsigned flags); */
  /* fftw_plan fftw_plan_many_r2r(int rank, const int *n, int howmany, */
  /*                              double *in, const int *inembed, */
  /*                              int istride, int idist, */
  /*                              double *out, const int *onembed, */
  /*                              int ostride, int odist, */
  /*                              const fftw_r2r_kind *kind, unsigned flags); */
  // https://github.com/chapel-lang/chapel/issues/13319
  pragma "default intent is ref"
  @chpldoc.nodoc
  record FFTWplan {
    param ftType : FFTtype;
    var plan : fftw_plan;

    // Mimic the advanced interface
    proc init(param ftType : FFTtype, args ...?k) {
      this.ftType = ftType;
      init this;
      plannerLock.lock();
      select ftType {
        when FFTtype.DFT do plan = fftw_plan_many_dft((...args));
        when FFTtype.R2R do plan = fftw_plan_many_r2r((...args));
      }
      plannerLock.unlock();
    }

    proc init=(other) {
      // to enable copying, we need to duplicate or borrow `other.plan`
      compilerError("copying of a FFTWplan is not supported");
    }

    proc deinit() {
      plannerLock.lock();
      destroy_plan(plan);
      plannerLock.unlock();
    }

    proc execute() {
      FFTW.execute(plan);
    }

    proc execute(arr1 : c_ptr(?T), arr2 : c_ptr(T)) {
      select ftType {
        when FFTtype.DFT do fftw_execute_dft(plan, arr1, arr2);
        when FFTtype.R2R do fftw_execute_r2r(plan, arr1, arr2);
      }
    }

    inline proc execute(ref arr1 : ?T, ref arr2 : T) where (T != c_ptr(?)) {
      this.execute(c_ptrTo(arr1), c_ptrTo(arr2));
    }

    inline proc execute(ref arr1 : ?T) where (T != c_ptr(?)) {
      this.execute(arr1, arr1);
    }

    proc isValid : bool {
      return plan != nil;
    }
  }

  /*
      Construct a distributed slab distributed domain.
      These are currently backed by BlockDist.

      :arg dom: Input 3D rectangular domain

      :returns: Returns a slab-distributed domain.
  */
  proc newSlabDom(dom: domain(?)) where dom.isRectangular() {
    if dom.rank != 3 then compilerError("The domain must be 3D");
    const targetLocales = reshape(Locales, {0.. #numLocales, 0..0, 0..0});
    return dom dmapped new blockDist(boundingBox=dom, targetLocales=targetLocales);
  }

  /*
      Construct a distributed slab distributed domain.
      These are currently backed by BlockDist.

      :arg sz: Size tuple ``(NX, NY, NZ)``

      :returns: Returns a slab-distributed domain.
  */
  proc newSlabDom(sz) where isHomogeneousTupleType(sz.type) {
    if sz.size != 3 then compilerError("The tuple must be 3D");
    var (x, y, z) = sz;
    return newSlabDom({0..#x, 0..#y, 0..#z});
  }

  /*
      Do the FFT.

      :arg ftType: Type of transform
      :type ftType: FFTtype
      :arg src: Input array [XYZ]
      :arg dst: Output array, transposed [YXZ]
      :arg signOrKind: Sign (for DFT), or kind (for R2R) of transform.

      Refer to the FFTW documentation for the different
      signs/kinds.

      Note that if ``ftType`` is ``FFTtype.R2R``, then
      the ``signOrKind`` argument must be a 3 element array
      (the type for each dimension separately).
  */
  proc doFFT_Transposed(param ftType : FFTtype,
                        src: [?SrcDom] ?T,
                        ref dst : [?DstDom] T,
                        signOrKind) {
    if (usePerformant) {
      doFFT_Transposed_Performant(ftType, src, dst, signOrKind);
    } else {
      doFFT_Transposed_Naive(ftType, src, dst, signOrKind);
    }
  }


  /* FFT.

     Stores the FFT in Dst transposed (xyz -> yxz).

     This implementation closely follows a pencil-paper
     algorithm, but is not very performant.
   */
  @chpldoc.nodoc
  proc doFFT_Transposed_Naive(param ftType : FFTtype,
                              Src: [?SrcDom] ?T,
                              Dst : [?DstDom] T,
                              signOrKind) {
    checkDims(SrcDom, DstDom);

    coforall loc in Locales do on loc {
      var timeTrack = new TimeTracker();

      const (xSrc, ySrc, zSrc) = SrcDom.localSubdomain().dims();
      const (yDst, xDst, _) = DstDom.localSubdomain().dims();

      // Set up FFTW plans
      var xPlan = setupPlan(T, ftType, {xDst, zSrc}, parDim=2, 1, signOrKind, FFTW_MEASURE);
      var yPlan = setupPlan(T, ftType, {ySrc, zSrc}, parDim=2, 1, signOrKind, FFTW_MEASURE);
      var zPlan = setupPlan(T, ftType, {0..0, zSrc}, parDim=1, 1, signOrKind, FFTW_MEASURE);

      // Use temp work array to avoid overwriting the Src array
      var myplane : [{0..0, ySrc, zSrc}] T;

      for ix in xSrc {
        // Copy source to temp array
        myplane = Src[{ix..ix, ySrc, zSrc}];

        // Y-transform
        timeTrack.start();
        forall iz in zSrc {
          yPlan.execute(myplane[0, ySrc.first, iz]);
        }
        timeTrack.stop(TimeStages.Y);

        // Z-transform, offset to reduce comm congestion/collision
        timeTrack.start();
        forall iy in offset(ySrc) {
          zPlan.execute(myplane[0, iy, zSrc.first]);
          // Transpose data into Dst
          Dst[{iy..iy, ix..ix, zSrc}] = myplane[{0..0, iy..iy, zSrc}];
        }
        timeTrack.stop(TimeStages.Z);
      }

      // Wait until all communication is complete
      allLocalesBarrier.barrier();

      // X-transform
      timeTrack.start();
      forall (iy, iz) in {yDst, zSrc} {
        xPlan.execute(Dst[iy, xDst.first, iz]);
      }
      timeTrack.stop(TimeStages.X);
    }
  }


  /* FFT.

     Stores the FFT in Dst transposed (xyz -> yxz).

     Performant version of the algorithm.
   */
  @chpldoc.nodoc
  proc doFFT_Transposed_Performant(param ftType : FFTtype,
                                   Src: [?SrcDom] ?T,
                                   ref Dst : [?DstDom] T,
                                   signOrKind) {
    checkDims(SrcDom, DstDom);

    coforall loc in Locales do on loc {
      var timeTrack = new TimeTracker();

      const (xSrc, ySrc, zSrc) = SrcDom.localSubdomain().dims();
      const (yDst, xDst, _) = DstDom.localSubdomain().dims();
      const myLineSize = zSrc.size*numBytes(T);

      // Setup FFTW plans
      var xPlan = setupBatchPlan(T, ftType, {xDst, zSrc}, parDim=2, signOrKind, FFTW_MEASURE);
      var yPlan = setupBatchPlan(T, ftType, {ySrc, zSrc}, parDim=2, signOrKind, FFTW_MEASURE);
      var zPlan = setupPlan(T, ftType, {0..0, zSrc}, parDim=1, 1, signOrKind, FFTW_MEASURE);

      // Use temp work array to avoid overwriting the Src array
      var myplane : [{0..0, ySrc, zSrc}] T;

      if usePrimitiveComm {
        forall iy in ySrc with (ref myplane) {
          copy(myplane[0, iy, zSrc.first], Src[xSrc.first, iy, zSrc.first], myLineSize);
        }
      } else {
        myplane = Src[{xSrc.first..xSrc.first, ySrc, zSrc}];
      }

      for ix in xSrc {
        // Y-transform
        timeTrack.start();
        forall (plan, myzRange) in yPlan.batch() with (ref myplane) {
          plan.execute(myplane[0, ySrc.first, myzRange.first]);
        }
        timeTrack.stop(TimeStages.Y);

        // Z-transform, offset to reduce comm congestion/collision
        timeTrack.start();
        forall iy in offset(ySrc) with (ref Dst, ref myplane) {
          zPlan.execute(myplane[0, iy, zSrc.first]);
          // Transpose data into Dst, and copy the next Src slice into myplane
          if usePrimitiveComm {
            copy(Dst[iy, ix, zSrc.first], myplane[0, iy, zSrc.first], myLineSize);
          } else {
            Dst[{iy..iy, ix..ix, zSrc}] = myplane[{0..0, iy..iy, zSrc}];
          }
          if (ix != xSrc.last) {
            if usePrimitiveComm {
              copy(myplane[0, iy, zSrc.first], Src[ix+1, iy, zSrc.first], myLineSize);
            } else {
              myplane[{0..0, iy..iy, zSrc}] = Src[{ix+1..ix+1, iy..iy, zSrc}];
            }
          }
        }
        timeTrack.stop(TimeStages.Z);
      }

      // Wait until all communication is complete
      allLocalesBarrier.barrier();

      // X-transform
      timeTrack.start();
      forall (plan, myzRange) in xPlan.batch() with (ref Dst) {
        for iy in yDst {
          plan.execute(Dst[iy, xDst.first, myzRange.first]);
        }
      }
      timeTrack.stop(TimeStages.X);
    }
  }

  /*
      Iterate over the range ``r`` but in an offset manner based
      on the locale id.
  */
  iter offset(r: range): int { halt("Serial offset not implemented"); }

  @chpldoc.nodoc
  iter offset(param tag: iterKind, r: range) where (tag==iterKind.standalone) {
    forall i in r + (r.size/numLocales * here.id) do {
      yield i % r.size + r.first;
    }
  }

  /*
      Low-level copy routine for contiguous data (a generalized ``memcpy``).

      :arg dst: Starting element of destination block
      :arg src: Starting element of source block
      :arg numBytes: Number of bytes to copy.

      Note that both ``dst`` and ``src`` cannot be remote.
  */
  proc copy(ref dst, const ref src, numBytes: int) {
    use Communication;
    const dstLocaleId = dst.locale.id;
    const srcLocaleId = src.locale.id;
    if dst.locale.id == here.id {
      get(c_ptrTo(dst), c_ptrToConst(src), srcLocaleId, numBytes.safeCast(c_size_t));
    } else if src.locale.id == here.id {
      put(c_ptrTo(dst), c_ptrToConst(src), dstLocaleId, numBytes.safeCast(c_size_t));
    } else {
      halt("Remote src and remote dst not yet supported");
    }
  }

  pragma "default intent is ref"
  @chpldoc.nodoc
  record BatchedFFTWplan {
    param ftType : FFTtype;
    const parRange: range;
    const numTasks: int;
    const batchSizeSm, batchSizeLg: int;
    var planSm, planLg: FFTWplan(ftType);

    proc init(type arrType, param ftType : FFTtype, dom : domain(2), parDim : int, signOrKind, in flags : c_uint) {
      this.ftType = ftType;
      const (dim1, dim2) = dom.dims();
      this.parRange = if parDim == 2 then dim2 else dim1;
      this.numTasks = min(here.maxTaskPar, parRange.size);
      this.batchSizeSm = parRange.size/numTasks;
      this.batchSizeLg = parRange.size/numTasks+1;
      this.planSm = setupPlan(arrType, ftType, dom, parDim, batchSizeSm, signOrKind, flags);
      this.planLg = setupPlan(arrType, ftType, dom, parDim, batchSizeLg, signOrKind, flags);
    }

    iter batch(): int {
      halt("Serial iterator not implemented");
    }

    /* This pragma makes the iterator yield `planSm` and `planLg` by reference,
       as they do not support copying. Alternatively, to yield by value,
       we could implement FFTWplan.init=() to either duplicate or borrow
       the fftw_plan object that FFTWplan points at. */
    pragma "do not unref for yields"
    iter ref batch(param tag : iterKind) where (tag==iterKind.standalone) {
      coforall chunk in chunks(parRange, numTasks) {
        if chunk.size == batchSizeSm then yield (planSm, chunk);
        if chunk.size == batchSizeLg then yield (planLg, chunk);
      }
    }
  }

  @chpldoc.nodoc
  proc setupBatchPlan(type arrType, param ftType : FFTtype, dom : domain(2), parDim : int, signOrKind, in flags : c_uint) {
    return new BatchedFFTWplan(arrType, ftType, dom, parDim, signOrKind, flags);
  }


  // Set up many 1D in place plans on a 2D array
  @chpldoc.nodoc
  proc setupPlan(type arrType, param ftType : FFTtype, dom : domain(2), parDim : int, numTransforms : int, signOrKind, in flags : c_uint) {
    // Pull signOrKind locally since this may be an array
    // we need to take a pointer to.
    var mySignOrKind = signOrKind;
    var arg0 : _signOrKindType(ftType);
    select ftType {
      when FFTtype.R2R do arg0 = c_ptrTo(mySignOrKind);
      when FFTtype.DFT do arg0 = mySignOrKind;
    }

    // Write down all the parameters explicitly
    var howmany = numTransforms : c_int;
    var nn : c_array(c_int, 1);
    var nnp = c_ptrTo(nn[0]);
    var rank = 1 : c_int;
    var stride, idist : c_int;
    const (dim1, dim2) = dom.dims();
    if (parDim == 2) {
      // FFT columns
      nn[0] = dim1.size : c_int;
      stride = dim2.size  : c_int;
      idist = 1 : c_int;
    } else {
      assert(parDim==1, "parDim can only be 1 or 2");
      nn[0] = dim2.size : c_int;
      stride = 1  : c_int;
      idist = dim2.size : c_int;
    }
    flags = flags | FFTW_UNALIGNED;

    // See if we already have wisdom for this plan. If we do, we avoid an
    // allocation.
    var arr: c_ptr(arrType);
    var plan = new FFTWplan(ftType, rank, nnp, howmany, arr,
                            nnp, stride, idist,
                            arr, nnp, stride, idist,
                            arg0, flags | FFTW_WISDOM_ONLY);

    if plan.isValid {
      return plan;
    } else {
      arr = allocate(arrType, dom.size.safeCast(c_size_t));
      defer { deallocate(arr); }
      return new FFTWplan(ftType, rank, nnp, howmany, arr,
                          nnp, stride, idist,
                          arr, nnp, stride, idist,
                          arg0, flags);
    }
  }

  // I could not combine these, so keep them separate for now.
  private proc _signOrKindType(param ftType : FFTtype) type
    where (ftType==FFTtype.DFT) {
    return c_int;
  }
  private proc _signOrKindType(param ftType : FFTtype) type
    where (ftType==FFTtype.R2R) {
    return c_ptr(fftw_r2r_kind);
  }

  private inline proc checkDims(SrcDom, DstDom) {
    if SrcDom.rank != 3 || DstDom.rank != 3 then compilerError("Code is designed for 3D arrays only");
    const (xSrc, ySrc, zSrc) = SrcDom.dims();
    const (xDst, yDst, zDst) = DstDom.dims();
    if xSrc != yDst then halt("Mismatched x-y ranges");
    if ySrc != xDst then halt("Mismatched y-x ranges");
    if zSrc != zDst then halt("Mismatched z ranges");
  }

  @chpldoc.nodoc
  module FFT_Locks {
    // https://github.com/chapel-lang/chapel/issues/9881
    // https://github.com/chapel-lang/chapel/issues/12300
    private use ChapelLocks;
    pragma "locale private"
    var plannerLock : chpl_LocalSpinlock;
  }

  @chpldoc.nodoc
  module FFT_Timers {
    use Time;
    // Time the various FFT steps.
    config const timeTrackFFT=false;

    enum TimeStages {X, Y, Z};
    const stageDomain = {TimeStages.X..TimeStages.Z};
    private var _globalTimeArr : [stageDomain] atomic real;

    resetTimers();

    proc deinit() {
      if timeTrackFFT then printTimers();
    }

    proc resetTimers() {
      for stage in _globalTimeArr do stage.write(0.0);
    }

    proc printTimers() {
      writeln("--------- Timings ---------------");
      for stage in TimeStages {
        writef("Time for %s : %10.2dr\n",stage:string, _globalTimeArr[stage].read());
      }
      writeln("---------------------------------");
    }


    record TimeTracker {
      var tt : stopwatch;
      var arr : [stageDomain] real;

      proc deinit() {
        if timeTrackFFT {
          for istage in arr.domain {
            _globalTimeArr[istage].add(arr[istage]);
          }
        }
      }

      proc ref start() {
        if timeTrackFFT {
          tt.clear(); tt.start();
        }
      }

      proc ref stop(stage) {
        if timeTrackFFT {
          tt.stop();
          arr[stage] += tt.elapsed();
        }
      }
    }

  }


  // End of module
}
