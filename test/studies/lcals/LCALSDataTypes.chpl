module LCALSDataTypes {
  use LCALSParams;
  use LCALSEnums;
  config const printLoopKernelIDs = false;

  class vector {
    type eltType;
    var D = {0..-1};
    var A: [D] eltType;
    proc this(i: int) ref {
      return A[i];
    }
    proc push_back(e: eltType) {
      A.push_back(e);
    }
    proc size() {
      return A.numElements;
    }
    proc resize(size: int) {
      D = {0..#size};
    }
    iter these() {
      for a in A do yield a;
    }
  }

  class LoopSuiteRunInfo {
    var host_name: string;

    var num_loops: int;
    var loop_names = new vector(string);

    var num_loop_lengths: int;
    var run_loop_length = new vector(bool);
    var loop_length_names = new vector(string);

    var num_suite_passes: int;
    var loop_samp_frac: real;

    var ref_loop_stat: LoopStat;

    var loop_weights = new vector(real);

    var num_loops_run = new vector(vector(int));
    var tot_time = new vector(vector(real));
    var fom_rel = new vector(vector(real));
    var fom_rate = new vector(vector(real));

    var cache_flush_data_len: int;
    var cache_flush_data_dom: domain(1);
    var cache_flush_data: [cache_flush_data_dom] real;
    var cache_flush_data_sum: real;

    var loop_test_stats_dom: domain(string);
    var loop_test_stats: [loop_test_stats_dom] vector(LoopStat);

    proc getLoopStats(loop_variant_name: string) {
      return loop_test_stats[loop_variant_name];
    }
    proc addLoopStats(name: string) {
      loop_test_stats_dom += name;
      loop_test_stats[name] = new vector(LoopStat);
    }
  }

  class LoopStat {
    var loop_is_run: bool;
    var loop_weight: real;

    var loop_run_time = new vector(vector(real));
    var loop_run_count = new vector(int);
    var mean = new vector(real);
    var std_dev = new vector(real);
    var min = new vector(real);
    var max = new vector(real);
    var harm_mean = new vector(real);
    var meanrel2ref = new vector(real);

    var loop_length = new vector(int);
    var samples_per_pass = new vector(int);

    var loop_chksum = new vector(real);

    proc LoopStat(num_loop_lengths: int) {
      loop_is_run = false;
      loop_weight = 0.0;

      loop_run_time.resize(num_loop_lengths);
      for i in 0..#num_loop_lengths do
        loop_run_time[i] = new vector(real);
      loop_run_count.resize(num_loop_lengths);
      mean.resize(num_loop_lengths);
      std_dev.resize(num_loop_lengths);
      min.resize(num_loop_lengths);
      max.resize(num_loop_lengths);
      harm_mean.resize(num_loop_lengths);
      meanrel2ref.resize(num_loop_lengths);

      loop_length.resize(num_loop_lengths);
      samples_per_pass.resize(num_loop_lengths);

      loop_chksum.resize(num_loop_lengths);
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

    proc ADomain(ilen: LoopLength, ndims: int) {
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
      for i in 0..#nnalls do real_zones[i] = -1;

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
    var scalar_Real: [0..#s_num_Real_scalars] real;

    // 
    var RealArray_1D: [0..#s_num_1D_Real_arrays][0..#aligned_chunksize] real;
    var RealArray_1D_Nx4: [0..#s_num_1D_Nx4_Real_arrays][0..#4*aligned_chunksize] real;
    var IndxArray_1D: [0..#s_num_1D_Indx_arrays][0..#aligned_chunksize] int;
    var ComplexArray_1D: [0..#s_num_1D_Complex_arrays][0..#aligned_chunksize] complex;

    var RealArray_2D_Nx25: [0..#s_num_2D_Nx25_Real_arrays][0..#aligned_chunksize, 0..#25] real;
    var RealArray_2D_7xN: [0..#s_num_2D_7xN_Real_arrays][0..#7, 0..#aligned_chunksize] real;
    var RealArray_2D_64x64: [0..#s_num_2D_64x64_Real_arrays][0..#64, 0..#64] real;
    var RealArray_3D_2xNx4: [0..#s_num_3D_2xNx4_Real_arrays][0..#2, 0..#aligned_chunksize, 0..#4] real;

    var RealArray_scalars: [0..#s_num_Real_scalars] real;
  }
}
