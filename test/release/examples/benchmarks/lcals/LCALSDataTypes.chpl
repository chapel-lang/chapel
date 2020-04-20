module LCALSDataTypes {
  public use LCALSParams;
  public use LCALSEnums;
  use List;

  //
  // This is a vector wrapper that uses 0-based indexing.
  //
  class vector {
    type eltType;
    var A: list(eltType);

    //
    // This vector supports 0-based indexing.
    //
    proc this(i: int) ref {
      return A[i];
    }
    proc push_back(e: eltType) {
      A.append(e);
    }
    proc size {
      return A.size;
    }
    iter these() {
      for a in A do yield a;
    }
  }

  class LoopSuiteRunInfo {
    var host_name: string;

    const loop_kernel_dom = {LoopKernelID.REF_LOOP..LoopKernelID.FIND_FIRST_MIN};
    const loop_length_dom = {LoopLength.LONG..LoopLength.SHORT};
    const loop_variants = LoopVariantID.RAW..LoopVariantID.FORALL_HYBRID_LAMBDA_TYPEFIX;
    const weight_group_dom = {WeightGroup.DATA_PARALLEL..WeightGroup.NUM_WEIGHT_GROUPS};;

    var loop_names: [loop_kernel_dom] string;

    var run_loop_length: [loop_length_dom] bool;
    var loop_length_names: [loop_length_dom] string;

    var num_suite_passes: int;
    var loop_samp_frac: real;

    const ref_loop_stat = new LoopStat();

    var loop_weights: [weight_group_dom] real;

    var cache_flush_data_len: int;
    var cache_flush_data_dom: domain(1);
    var cache_flush_data: [cache_flush_data_dom] real;
    var cache_flush_data_sum: real;

    var loop_test_stats = for loop_variants do [loop_kernel_dom] new shared LoopStat();

    proc getLoopStats(loop_variant: LoopVariantID) ref {
      return loop_test_stats[loop_variant];
    }
  }

  class LoopStat {
    var loop_is_run: bool;
    var loop_weight: real;
    var loop_length_dom = {LoopLength.LONG..LoopLength.SHORT};

    var loop_run_time: [loop_length_dom] owned vector(real);
    var loop_run_count: [loop_length_dom] int;
    var mean: [loop_length_dom] real;
    var std_dev: [loop_length_dom] real;
    var min: [loop_length_dom] real;
    var max: [loop_length_dom] real;
    var harm_mean: [loop_length_dom] real;
    var meanrel2ref: [loop_length_dom] real;

    var loop_length: [loop_length_dom] int;
    var samples_per_pass: [loop_length_dom] int;

    var loop_chksum: [loop_length_dom] real;

    proc init() {
      loop_run_time = for i in loop_length_dom do new vector(real);
    }
  }

  record ADomain {
    /*static*/ var loop_length_factor: real = 1.0;

    var ndims: int;
    var NPNL: int;
    var NPNR: int;

    var imin: int;
    var jmin: int;
    var kmin: int;
    var imax: int;
    var jmax: int;
    var kmax: int;

    var jp: int;
    var kp: int;
    var nnalls: int;

    var fpn: int;
    var lpn: int;
    var frn: int;
    var lrn: int;

    var fpz: int;
    var lpz: int;

    var zoneDom = {0..-1};
    var real_zones: [zoneDom] int;
    var n_real_zones: int;

    proc init(ilen: LoopLength, ndims: int) {
      var rzmax: int;
      this.ndims = ndims;
      NPNL = 2;
      NPNR = 1;
      select ilen {
        when LoopLength.LONG {
          if ndims == 2 {
            rzmax = (loop_length_factor*156):int;
          } else if ndims == 3 {
            rzmax = (loop_length_factor*28):int;
          }
        }
        when LoopLength.MEDIUM {
          if ndims == 2 {
            rzmax = (loop_length_factor*64):int;
          } else if ndims == 3 {
            rzmax = (loop_length_factor*16):int;
          }
        }
        when LoopLength.SHORT {
          if ndims == 2 {
            rzmax = (loop_length_factor*8):int;
          } else if ndims == 3 {
            rzmax = (loop_length_factor*4):int;
          }
        }
        otherwise {
          halt("bad LoopLength");
          // error
        }
      }

      imin = NPNL;
      jmin = NPNL;
      imax = rzmax + NPNR;
      jmax = rzmax + NPNR;
      jp = imax - imin + 1 + NPNL + NPNR;
      if ndims == 2 {
        kmin = 0;
        kmax = 0;
        kp = 0;
        nnalls = jp * (jmax - jmin + 1 + NPNL + NPNR);
      } else if ndims == 3 {
        kmin = NPNL;
        kmax = rzmax + NPNR;
        kp = jp * (jmax - jmin + 1 + NPNL + NPNR);
        nnalls = kp * (kmax - kmin + 1 + NPNL + NPNR);
      }

      fpn = 0;
      lpn = nnalls - 1;
      frn = fpn + NPNL * (kp + jp) + NPNL;
      lrn = lpn - NPNR * (kp + jp) - NPNR;

      fpz = frn - jp - kp - 1;
      lpz = lrn;

      zoneDom = {0..#nnalls};
      real_zones = -1;

      n_real_zones = 0;

      if ndims == 2 {
        for j in jmin..jmax-1 {
          for i in imin..imax-1 {
            var ip = i + j*jp ;

            var id = n_real_zones;
            real_zones[id] = ip;
            n_real_zones += 1;
          }
        }
      } else if ndims == 3 {
        for k in kmin..kmax-1 {
          for j in jmin..jmax-1 {
            for i in imin..imax-1 {
              var ip = i + j*jp + kp*k ;

              var id = n_real_zones;
              real_zones[id] = ip;
              n_real_zones += 1;
            }
          }
        }
      }
    }
  }


  record RealArray {
    const id: int;
    const len: int;
    var data: [0..#len] real;
  }

  record IndxArray {
    const id: int;
    const len: int;
    var data: [0..#len] int;
  }

  record ComplexArray {
    const id: int;
    const len: int;
    var data: [0..#len] int;
  }


  class LoopData {
    var max_loop_length: int;
    const num_aligned_segments = (max_loop_length + 20)/LCALS_DATA_ALIGN + 1;
    const aligned_chunksize = num_aligned_segments * LCALS_DATA_ALIGN;

    const s_num_1D_Real_arrays = 16;
    const s_num_1D_Nx4_Real_arrays = 2;
    const s_num_1D_Indx_arrays = 5;
    const s_num_1D_Complex_arrays = 5;

    const s_num_2D_Nx25_Real_arrays = 4;
    const s_num_2D_7xN_Real_arrays = 11;
    const s_num_2D_64x64_Real_arrays = 1;

    const s_num_3D_2xNx4_Real_arrays = 3;
    const s_num_Real_scalars = 10;

    // pointer based versions of arrays

    var RealArray_1D: [0..#s_num_1D_Real_arrays][0..#aligned_chunksize] real;
    var RealArray_1D_Nx4: [0..#s_num_1D_Nx4_Real_arrays][0..#4*aligned_chunksize] real;
    var IndxArray_1D: [0..#s_num_1D_Indx_arrays][0..#aligned_chunksize] int;
    var ComplexArray_1D: [0..#s_num_1D_Complex_arrays][0..#aligned_chunksize] complex;

    var RealArray_2D_Nx25: [0..#s_num_2D_Nx25_Real_arrays][0..#aligned_chunksize, 0..#25] real;
    var RealArray_2D_7xN: [0..#s_num_2D_7xN_Real_arrays][0..#7, 0..#aligned_chunksize] real;
    var RealArray_2D_64x64: [0..#s_num_2D_64x64_Real_arrays][0..#64, 0..#64] real;

    // This is how this array *seems* like it should be declared, making
    // it an array of 3D arrays of 2xNx4 elements.  However, the reference
    // LCALS uses a bunch of pointers into a data vector that cause many
    // indices to overlap with each other. The LCALS_Overlapping_Array_3D
    // class implements the same pattern used in the LCALS reference.
    //
    // var RealArray_3D_2xNx4: [0..#s_num_3D_2xNx4_Real_arrays][0..#2, 0..#aligned_chunksize, 0..#4] real;
    var RealArray_3D_2xNx4: [0..#s_num_3D_2xNx4_Real_arrays] owned LCALS_Overlapping_Array_3D(real) = [i in 0..#s_num_3D_2xNx4_Real_arrays] new LCALS_Overlapping_Array_3D(real, 2*4*aligned_chunksize); // 2 X loop_length X 4 array size

    var RealArray_scalars: [0..#s_num_Real_scalars] real;
  }

  /* Mimic the strange, self-overlapping 3D array in the LCALS
     benchmark reference.

     The 3D array there is actually a 2D array of pointers into the
     array "data", with each pointer representing 4 elements.
     A[i,j] = &data[i*j*4]

     This means that many of these 4 element pointers will overlap,
     for example i==0 or j==0, will always point to the beginning of
     "data".

     data:
      01234567890123456
     [A   B   C   D   E]

     2D_Array:
      01234
   0 [AAAAA]
   1 [ABCDE]

     The letter in "2D_Array" is the letter that element points to within
     "data". In the full 3 dimensions, A[i,j,k] points at data[i*j*4 + k].
   */
  class LCALS_Overlapping_Array_3D {
    type t;
    var len: int;
    var data: [0..#len] t;
    proc this(i: int, j: int, k: int) ref {
      return data[i*j*4 + k];
    }
    // iterate through the data array instead of following the strange
    // access pattern. This is just used to initialize data and compute
    // checksums.
    iter these() ref {
      for i in 0..#len {
        yield data[i];
      }
    }
  }

}
