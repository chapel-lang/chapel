module main {
  use FileSystem;
  use LCALSParams;
  use LCALSEnums;
  use LCALSDataTypes;
  use LCALSLoops;
  use LCALSStatic;

  use RunARawLoops;
  use RunBRawLoops;
  use RunCRawLoops;
  proc allocateLoopData() {
    const num_aligned_segments =
      (s_loop_data.max_loop_length + 20)/LCALS_DATA_ALIGN + 1;
    const aligned_chunksize = num_aligned_segments*LCALS_DATA_ALIGN;

    writeln("\n allocateLoopData...");

    for i in 0..#s_loop_data.s_num_1D_Real_arrays {
      initData(s_loop_data.RealArray_1D[i], i+1);
    }
    for i in 0..#s_loop_data.s_num_1D_Nx4_Real_arrays {
      initData(s_loop_data.RealArray_1D_Nx4[i], i+1);
    }
    for i in 0..#s_loop_data.s_num_1D_Indx_arrays {
      initData(s_loop_data.IndxArray_1D[i], i+1);
    }
    for i in 0..#s_loop_data.s_num_1D_Complex_arrays {
      initData(s_loop_data.ComplexArray_1D[i], i+1);
    }
    for i in 0..#s_loop_data.s_num_2D_Nx25_Real_arrays {
      initData(s_loop_data.RealArray_2D_Nx25[i], i+1);
    }
    for i in 0..#s_loop_data.s_num_2D_7xN_Real_arrays {
      initData(s_loop_data.RealArray_2D_7xN[i], i+1);
    }
    for i in 0..#s_loop_data.s_num_2D_64x64_Real_arrays {
      initData(s_loop_data.RealArray_2D_64x64[i], i+1);
    }
    for i in 0..#s_loop_data.s_num_3D_2xNx4_Real_arrays {
      initData(s_loop_data.RealArray_3D_2xNx4[i], i+1);
    }
    initData(s_loop_data.scalar_Real, 21);
    // ?? s_loop_data->RealArray_scalars;
  }


  proc main {
    const num_suite_passes = 1;
    const sample_frac = 1.0;
    const loop_length_factor = 1.0;
    var run_loop_length: [0..#LoopLength.NUM_LENGTHS] bool = true;
    var run_loop: [0..#LoopKernelID.NUM_LOOP_KERNELS] bool = false;
    var run_variants = new vector(LoopVariantID);

    run_loop[LoopKernelID.REF_LOOP] = true;

    // Loop Subset A: Loops extracted from LLNL app codes.
    run_loop[LoopKernelID.PRESSURE_CALC ] = false; //true; // M and S have bad checksums
    run_loop[LoopKernelID.ENERGY_CALC   ] = true;
    run_loop[LoopKernelID.VOL3D_CALC    ] = true;
    run_loop[LoopKernelID.DEL_DOT_VEC_2D] = true;
    run_loop[LoopKernelID.COUPLE        ] = true;
    run_loop[LoopKernelID.FIR           ] = false; //true; // bad checksums

    // Loop Subset B: "Basic" Loops.
    run_loop[LoopKernelID.INIT3         ] = true;
    run_loop[LoopKernelID.MULADDSUB     ] = true;
    run_loop[LoopKernelID.IF_QUAD       ] = true;
    run_loop[LoopKernelID.TRAP_INT      ] = true;

    // Loop Subset C: Loops from older Livermore Loops in "C" suite.
    run_loop[LoopKernelID.HYDRO_1D      ] = true;
    run_loop[LoopKernelID.ICCG          ] = false; //true; // segfaults
    run_loop[LoopKernelID.INNER_PROD    ] = true;
    run_loop[LoopKernelID.BAND_LIN_EQ   ] = false; //true; // bad checksums
    run_loop[LoopKernelID.TRIDIAG_ELIM  ] = false; //true; // bad checksums
    run_loop[LoopKernelID.EOS           ] = false; //true; // bad checksums
    run_loop[LoopKernelID.ADI           ] = false; // not implemented
    run_loop[LoopKernelID.INT_PREDICT   ] = false; // not implemented
    run_loop[LoopKernelID.DIFF_PREDICT  ] = false; // not implemented
    run_loop[LoopKernelID.FIRST_SUM     ] = false; //true; ???
    run_loop[LoopKernelID.FIRST_DIFF    ] = false; //true; bad checksums
    run_loop[LoopKernelID.PIC_2D        ] = false; // not implemented
    run_loop[LoopKernelID.PIC_1D        ] = false; //true; // bad checksums
    run_loop[LoopKernelID.HYDRO_2D      ] = false; // not implemented
    run_loop[LoopKernelID.GEN_LIN_RECUR ] = false; //true; // bad checksums
    run_loop[LoopKernelID.DISC_ORD      ] = false; //true; // bad checksums
    run_loop[LoopKernelID.MAT_X_MAT     ] = false; // not implemented
    run_loop[LoopKernelID.PLANCKIAN     ] = false; //true; // bad checksums
    run_loop[LoopKernelID.IMP_HYDRO_2D  ] = false; // not implemented
    run_loop[LoopKernelID.FIND_FIRST_MIN] = true;

    mkdir(output_dirname, parents=true);

    if !run_misc {
      run_variants.push_back(LoopVariantID.RAW);
      //run_variants.push_back(LoopVariantID.FORALL_LAMBDA);

      //run_variants.push_back(LoopVariantID.RAW_OMP);
      //run_variants.push_back(LoopVariantID.FORALL_LAMBDA_OMP);
    } else {
      run_variants.push_back(LoopVariantID.RAW);
      //run_variants.push_back(LoopVariantID.FORALL_LAMBDA);
    }
    const cache_size = 0;
    const host_name = here.name;
    writeln("\n Running loop suite on ", host_name);

    allocateLoopSuiteRunInfo(host_name,
                             LoopKernelID.NUM_LOOP_KERNELS,
                             LoopLength.NUM_LENGTHS,
                             num_suite_passes,
                             run_loop_length,
                             cache_size);

    defineLoopSuiteRunInfo(run_variants, run_loop,
                           sample_frac, loop_length_factor);
    allocateLoopData();

    if do_fom {
      computeReferenceLoopTimes();
    }

    for ipass in 0..#num_suite_passes {
      writeln("\n run suite: pass = ", ipass);
      for variant in run_variants {
        const loop_variant_name = getVariantName(variant);
        writeln("\t run loop variant ---> ", loop_variant_name);
        for ilen in 0..#LoopLength.NUM_LENGTHS {
          var rilen = ilen: LoopLength;
          if run_loop_length[ilen] {
            runLoopVariant(variant, run_loop, rilen);
          }
        }
      }
    }

    writeln("\n generate reports....");
    var run_variant_names = getVariantNames(run_variants);

    generateTimingReport(run_variant_names, output_dirname);
    generateChecksumReport(run_variant_names, output_dirname);
    generateFOMReport(run_variant_names, output_dirname);

    freeLoopData();
    writeln("\n freeLoopSuiteRunInfo...");
    freeLoopSuiteRunInfo();
    writeln("\n DONE!!! ");
  }

  proc computeStats(ilv: int, loop_stats: vector(LoopStat), do_fom: bool) {
    for iloop in 0..#loop_stats.size() {
      var stat = loop_stats[iloop];
      for ilen in 0..#stat.loop_length.size() {
        if stat.loop_run_count[ilen] > 0 {
          var time_sample = stat.loop_run_time[ilen];
          var sample_size = time_sample.size();
          var mean = 0.0;
          var sdev = 0.0;
          var maxv = min(real);
          var minv = max(real);
          var harm = 0.0;

          for is in 0..#sample_size {
            mean += time_sample[is];
            maxv = max(maxv, time_sample[is]);
            minv = min(minv, time_sample[is]);
            if time_sample[is] > 0.0 {
              harm += 1.0/time_sample[is];
            }
          }
          mean /= sample_size;
          if harm > 0.0 then harm = sample_size/harm;
          for is in 0..#sample_size {
            sdev += (time_sample[is] - mean) * (time_sample[is] - mean);
          }
          sdev /= sample_size;

          stat.mean[ilen]      = mean;
          stat.std_dev[ilen]   = sdev;
          stat.min[ilen]       = minv;
          stat.max[ilen]       = maxv;
          stat.harm_mean[ilen] = harm;
        }
      }
    }
    if do_fom {
      // FIXME: implement this
    }
  }

  proc generateTimingReport(run_loop_variants: vector(string),
                            output_dirname: string) {
    if run_loop_variants.size() == 0 then return;
    var do_fom = true;
    const ver_info = buildVersionInfo();
    var suite_run_info = getLoopSuiteRunInfo();
    const nvariants = run_loop_variants.size();
    for ilv in 0..#nvariants {
      computeStats(ilv, suite_run_info.getLoopStats(run_loop_variants[ilv]), do_fom);
    }
    if output_dirname.length != 0 {
      const timing_fname = output_dirname + "/" + "timing.txt";
      var outfile = open(timing_fname, iomode.cw);
      var writer = outfile.writer();

      writeTimingSummaryReport(run_loop_variants, writer);

      writer.close();
      outfile.close();

      for ilv in 0..#nvariants {
        writeMeanTimeReport(run_loop_variants[ilv], output_dirname);
      }
      for ilv in 1..nvariants-1 {
        writeRelativeTimeReport(run_loop_variants[ilv], output_dirname);
      }
      
    } else {
      writeTimingSummaryReport(run_loop_variants, stdout);
    }
  }

  proc writeMeanTimeReport(variant_name: string, output_dirname: string) {
    var rept_fname = output_dirname + "/" + variant_name + "-meantime.txt";
    var outfile = open(rept_fname, iomode.cw);
    var writer = outfile.writer();

    writeln("\n writeMeanTimeReport...   ", rept_fname);
    var suite_run_info = getLoopSuiteRunInfo();
    var loop_names = suite_run_info.loop_names;
    var len_names = suite_run_info.loop_length_names;
    const sepchr = " , ";
    const prec = 8;
    writer.write(variant_name, " Mean Run Times ");

    for i in 0..#len_names.size() {
      writer.write(sepchr);
    }
    writer.writeln();

    for i in 0..#len_names.size() {
      writer.write(sepchr, len_names[i]);
    }
    writer.writeln();

    for iloop in 0..#loop_names.size() {
      var stat = suite_run_info.getLoopStats(variant_name)[iloop];
      if loop_names[iloop].length != 0 && stat.loop_is_run {
        writer.write(loop_names[iloop]);
        for ilen in 0..#stat.loop_length.size() {
          writer.writef("%s%r", sepchr, stat.mean[ilen]);
        }
        writer.writeln();
      }
    }
    writer.close();
    outfile.close();
  }

  proc writeRelativeTimeReport(variant_name: string, output_dirname: string) {
    var rept_fname = output_dirname + "/" + variant_name + "-reltime.txt";
    var outfile = open(rept_fname, iomode.cw);
    var writer = outfile.writer();
    writer.writeln("\n writeRelativeTimeReport...   ");

    var suite_run_info = getLoopSuiteRunInfo();
    var loop_names = suite_run_info.loop_names;
    var len_names = suite_run_info.loop_length_names;

    const sepchr = " , ";
    const prec = 6;

    writer.write(variant_name, " Relative Run Times ");
    for i in 0..#len_names.size() {
      writer.write(sepchr);
    }
    writer.writeln();

    for i in 0..#len_names.size() {
      writer.write(sepchr, len_names[i]);
    }
    writer.writeln();

    for iloop in 0..#loop_names.size() {
      var stat = suite_run_info.getLoopStats(variant_name)[iloop];
      if loop_names[iloop].length != 0 && stat.loop_is_run {
        writer.write(loop_names[iloop]);
        for ilen in 0..#stat.loop_length.size() {
          writer.write(sepchr, stat.meanrel2ref[ilen]);
        }
        writer.writeln();
      }
    }
    writer.close();
    outfile.close();
  }

  proc writeTimingSummaryReport(run_loop_variants: vector(string), outchannel) {
    var suite_run_info = getLoopSuiteRunInfo();
    const nvariants = run_loop_variants.size();
    const ref_variant = run_loop_variants[0];
    var loop_names = suite_run_info.loop_names;

    const equal_line = "===========================================================================================================\n";
    const dash_line = "------------------------------------------------------------------------------------------------------------\n";
    const dash_line_part = "-------------------------------------------------------\n";
    const dot_line_part = "............................................\n";
    var len_id = new vector(string);
    len_id.resize(suite_run_info.loop_length_names.size());
    for ilen in 0..#len_id.size() {
      len_id[ilen] = suite_run_info.loop_length_names[ilen].substring(1);
    }

    var ver_info = buildVersionInfo();

    outchannel.write("\n\n\n");
    outchannel.write(equal_line);
    outchannel.write(equal_line);

    outchannel.writeln("LCALS compilation summary: ");
    outchannel.writeln(ver_info);

    outchannel.write("\n\n");
    outchannel.write(equal_line);
    outchannel.write(equal_line);

    outchannel.writeln("LCALS run summary: ");
    outchannel.writeln("sizeof(Real_type) = ", 8 /*sizeof(Real_type)*/);
    outchannel.writeln("     num suite passes = ", suite_run_info.num_suite_passes);
    outchannel.writeln("     loop sample fraction = ", suite_run_info.loop_samp_frac);
    outchannel.write("     loop variants run : ");
    for ilv in 0..#nvariants {
      var last_char: string;
      if ( ilv+1 < run_loop_variants.size() ) then last_char = " , ";
      outchannel.write(run_loop_variants[ilv], last_char);
    }
    outchannel.writeln("\n     reference variant : ", ref_variant);
    outchannel.write(equal_line);
    outchannel.write(equal_line);

    var max_name_len = 0;
    for iloop in 0..#loop_names.size() {
      max_name_len = max(max_name_len, loop_names[iloop].length);
    }
    var max_var_name_len = 0;
    for ilv in 0..#nvariants {
      max_var_name_len = max(max_var_name_len, run_loop_variants[ilv].length);
    }
    var var_field = "Variant(length id)";
    var var_field_len = var_field.length;
    var prec = 10;
    var prec_buf = prec + 8;
    var reldiff_prec = 6;

    outchannel.writeln("Loop name(Loop ID) -->   <length id>:(length, samples/pass), etc.");

    outchannel.writef("%-*s", var_field_len+1, var_field);
    outchannel.writef("%*s", prec_buf, "   Mean Time ");
    outchannel.writef("%-*s", prec_buf, "        Min Time");
    outchannel.writef("%-*s", prec_buf, "      Max Time");
    outchannel.writef("%-*s", prec_buf, "    Std. Dev.");
    outchannel.writef("%-*s\n", prec_buf, "Mean time rel to ref variant");
    outchannel.write(dash_line);

    //
    // Print timing results for all loops in a table.
    //

    for iloop in 0..#loop_names.size() {
      var ref_variant_stat = suite_run_info.getLoopStats(run_loop_variants[0])[iloop];

      var ref_mean = new vector(real);
      ref_mean.resize(ref_variant_stat.mean.size());
      for i in 0..#ref_mean.size() {
        ref_mean[i] = ref_variant_stat.mean[i];
      }

      if loop_names[iloop].length != 0 && ref_variant_stat.loop_is_run {
        if iloop > 1 {
          outchannel.write("\n", dash_line_part);
        }
        outchannel.writef("%s (%i) --> ", loop_names[iloop], iloop);
        for ilv in 0..#nvariants {
          var stat = suite_run_info.getLoopStats(run_loop_variants[ilv])[iloop];
          if stat.loop_is_run {
            if ilv == 0 {
              for ilen in 0..#stat.loop_length.size() {
                outchannel.writef("   %s:(%i, %i)", len_id[ilen], stat.loop_length[ilen], stat.samples_per_pass[ilen]);
              }
              outchannel.writeln();
            } else {
              outchannel.write(dot_line_part);
            }
            for ilen in 0..#stat.loop_length.size() {
              if stat.loop_run_count[ilen] > 0 {
                var var_string = run_loop_variants[ilv] + "(" + len_id[ilen] + ")";
                outchannel.writef("%-*s", prec, var_string);

                outchannel.writef("%*s", prec_buf, stat.mean[ilen]);
                outchannel.writef("%*s", prec_buf, stat.min[ilen]);
                outchannel.writef("%*s", prec_buf, stat.max[ilen]);
                outchannel.writef("%*s", prec_buf, stat.std_dev[ilen]);

                if ilv > 0 {
                  var rel_mean_diff = 0.0;
                  if ref_mean[ilen] != 0.0 {
                    rel_mean_diff = 1.0 + (stat.mean[ilen]-ref_mean[ilen])/ref_mean[ilen];
                  }
                  outchannel.writef("%*r", prec_buf, rel_mean_diff);
                  stat.meanrel2ref[ilen] = rel_mean_diff;
                } else {
                  outchannel.writeln();
                }
              }
            }
          }
        }
      }
    }
    outchannel.write(dash_line);
    outchannel.write("\n\n\n");
  }

  proc buildVersionInfo() {
    return "Chapel";
  }

  proc generateChecksumReport(run_loop_variants: vector(string),
                              output_dirname: string) {
    if run_loop_variants.size() == 0 then return;
    if output_dirname.length != 0 {
      var checksum_fname = output_dirname + "/" + "checksum.txt";
      var outfile = open(checksum_fname, iomode.cw);
      var writer = outfile.writer();
      writer.write("\n writeChecksumReport...    ", checksum_fname);
      writeChecksumReport(run_loop_variants, writer);
      writer.close();
      outfile.close();
    } else {
      writeChecksumReport(run_loop_variants, stdout);
    }
  }

  proc writeChecksumReport(run_loop_variants: vector(string), outchannel) {
    var suite_run_info = getLoopSuiteRunInfo();
    var nvariants = run_loop_variants.size();
    var ref_variant = run_loop_variants[0];
    var loop_names = suite_run_info.loop_names;
    var len_id = new vector(string);

    len_id.resize(suite_run_info.loop_length_names.size());
    for ilen in 0..#len_id.size() {
      len_id[ilen] = suite_run_info.loop_length_names[ilen].substring(1);
    }


    const equal_line = "===========================================================================================================\n";
    const dash_line = "------------------------------------------------------------------------------------------------------------\n";
    const dash_line_part = "-------------------------------------------------------\n";
    const dot_line_part = "............................................\n";

    var ver_info = buildVersionInfo();
    outchannel.write("\n\n\n");
    outchannel.write(equal_line);
    outchannel.write(equal_line);
    outchannel.writeln("LCALS compilation summary: ", ver_info);

    outchannel.write("\n\n");
    outchannel.write(equal_line);
    outchannel.write(equal_line);

    var max_name_len = 0;
    for iloop in 0..loop_names.size() {
      max_name_len = max(max_name_len, loop_names[iloop].length);
    }
    var max_var_name_len = 0;
    for ilv in 0..#nvariants {
      max_var_name_len = max(max_var_name_len, run_loop_variants[ilv].length);
    }

    var var_field = "Variant(length #)";
    var var_field_len = var_field.length;
    var prec = 32;
    var prec_buf = prec + 8;

    outchannel.writeln("Loop name -->");
    outchannel.writef("%-*s", var_field_len+1, var_field);
    outchannel.writef("%*s", prec_buf, "Check Sum    ");
    outchannel.writef("%-*s\n", prec_buf, "        Delta from reference");
    outchannel.write(dash_line);


    for iloop in 0..#loop_names.size() {
      var ref_variant_stat = suite_run_info.getLoopStats(run_loop_variants[0])[iloop];
      var ref_chksum = new vector(real);
      ref_chksum.resize(ref_variant_stat.loop_chksum.size());
      for i in 0..#ref_chksum.size() {
        ref_chksum[i] = ref_variant_stat.loop_chksum[i];
      }
      if loop_names[iloop].length != 0 && ref_variant_stat.loop_is_run {
        if iloop > 1 then
          outchannel.write("\n", dash_line_part);
        outchannel.write(loop_names[iloop], "(", iloop, ") --> ");
        for ilv in 0..#nvariants {
          var stat = suite_run_info.getLoopStats(run_loop_variants[ilv])[iloop];
          if stat.loop_is_run {

            if ilv == 0 then outchannel.writeln();
            else outchannel.write(dot_line_part);

            for ilen in 0..#stat.loop_length.size() {
              if stat.loop_run_count[ilen] > 0 {
                var var_string = run_loop_variants[ilv] + "(" + len_id[ilen] + ")";
                outchannel.writef("%-*s", var_field_len+1, var_string);
                //outchannel.writef("%*s", prec_buf, stat.loop_chksum[ilen]);
                outchannel.writef("#########.#######################", stat.loop_chksum[ilen]);
                if ilv > 0 {
                  var chksum_diff = abs(stat.loop_chksum[ilen] - ref_chksum[ilen]);
                  outchannel.writef("%*r\n", prec_buf, chksum_diff);
                } else {
                  outchannel.writeln();
                }
/*
                outchannel.writeln(stat.mean[ilen]);
                outchannel.writeln(stat.min[ilen]);
                outchannel.writeln(stat.max[ilen]);
                outchannel.writeln(stat.std_dev[ilen]);
                if ilv > 0 {
                  var rel_mean_diff = 0.0;
                  if ref_mean[ilen] != 0.0 {
                    rel_mean_diff = 1.0 + (stat.mean[ilen] - ref_mean[ilen])/ref_mean[ilen];
                  }
                  outchannel.writeln(rel_mean_diff);
                }
*/
              }
            }
          }
        }
      }
    }
    outchannel.write(dash_line);
    outchannel.write("\n\n\n");
  }

  proc generateFOMReport(run_loop_variants: vector(string),
                         output_dirname: string) {
  }

  proc freeLoopData() {
  }

  proc freeLoopSuiteRunInfo() {
  }

  proc runLoopVariant(lvid: LoopVariantID, run_loop:[] bool, ilength: LoopLength) {
    var loop_suite_run_info = getLoopSuiteRunInfo();
    const loop_variant_name = getVariantName(lvid);
    var loop_stats = loop_suite_run_info.getLoopStats(loop_variant_name);
    select lvid {
      when LoopVariantID.RAW {
        runARawLoops(loop_stats, run_loop, ilength);
        runBRawLoops(loop_stats, run_loop, ilength);
        runCRawLoops(loop_stats, run_loop, ilength);
      }
/*
      when LoopVariantID.FORALL_LAMBDA {
        runAForallLambdaLoops(loop_stats, run_loop, ilength);
        runBForallLambdaLoops(loop_stats, run_loop, ilength);
        runCForallLambdaLoops(loop_stats, run_loop, ilength);
      }
      when LoopVariantID.RAW_OMP {
        runOMPRawLoops(loop_stats, run_loop, ilength);
      }
      when LoopVariantID.FORALL_LAMBDA_OMP {
        runOMPForallLambdaLoops(loop_stats, run_loop, ilength);
      }
      when LoopVariantID.FORALL_HYBRID_LAMBDA {
        runAForallHybridLambdaLoops(loop_stats, run_loop, ilength);
        runBForallHybridLambdaLoops(loop_stats, run_loop, ilength);
        runCForallHybridLambdaLoops(loop_stats, run_loop, ilength);
      }
      when LoopVariantID.FORALL_FUNCTOR {
        runAForallFunctorLoops(loop_stats, run_loop, ilength);
        runBForallFunctorLoops(loop_stats, run_loop, ilength);
        runCForallFunctorLoops(loop_stats, run_loop, ilength);
      }
      when LoopVariantID.FORALL_FUNCTOR_OMP {
        runOMPForallFunctorLoops(loop_stats, run_loop, ilength);
      }
      when LoopVariantID.RAW_FUNC {
        runARawFuncLoops(loop_stats, run_loop, ilength);
        runBRawFuncLoops(loop_stats, run_loop, ilength);
        runCRawFuncLoops(loop_stats, run_loop, ilength);
      }
      when LoopVariantID.FORALL_LAMBDA_TYPEFIX {
        runAForallLambdaLoops_TYPEFIX(loop_stats, run_loop, ilength);
        runBForallLambdaLoops_TYPEFIX(loop_stats, run_loop, ilength);
        runCForallLambdaLoops_TYPEFIX(loop_stats, run_loop, ilength);
      }
      when LoopVariantID.FORALL_LAMBDA_OMP_TYPEFIX {
        runOMPForallLambdaLoops_TYPEFIX(loop_stats, run_loop, ilength);
      }
      when LoopVariantID.FORALL_HYBRID_LAMBDA_TYPEFIX {
        runAForallHybridLambdaLoops_TYPEFIX(loop_stats, run_loop, ilength);
        runBForallHybridLambdaLoops_TYPEFIX(loop_stats, run_loop, ilength);
        runCForallHybridLambdaLoops_TYPEFIX(loop_stats, run_loop, ilength);
      }
*/
      otherwise {
        halt("Unknown variant: ", lvid);
      }
    }
  }



  proc allocateLoopSuiteRunInfo(host_name: string,
                                num_loops: int,
                                num_loop_lengths: int,
                                num_suite_passes: int,
                                run_loop_length: [] bool,
                                cache_size: int) {
    extern proc drand48(): real;
    writeln("\n allocateLoopSuiteRunInfo...");

    s_loop_suite_run_info.host_name = host_name;
    s_loop_suite_run_info.num_loops = num_loops;
    s_loop_suite_run_info.num_loop_lengths = num_loop_lengths;
    s_loop_suite_run_info.num_suite_passes = num_suite_passes;
    for ilen in 0..#num_loop_lengths {
      s_loop_suite_run_info.run_loop_length.push_back(run_loop_length[ilen]);
    }
    const sizeofReal = 8;

    s_loop_suite_run_info.cache_flush_data_len = (cache_size*2)/sizeofReal; 
    s_loop_suite_run_info.cache_flush_data_dom = {0..#s_loop_suite_run_info.cache_flush_data_len};
    for a in s_loop_suite_run_info.cache_flush_data {
      a = drand48() + 0.1;
    }
  }

  proc getVariantName(lvid: LoopVariantID) {
    return lvid:string;
  }

  proc getVariantNames(lvids: vector(LoopVariantID)) {
    var run_variant_names = new vector(string);
    for lvid in lvids {
      run_variant_names.push_back(getVariantName(lvid));
    }
    return run_variant_names;
  }

  proc computeReferenceLoopTimes() {
    var suite_info = getLoopSuiteRunInfo();
    var ref_loop_stat = suite_info.ref_loop_stat;

    var lstat0 = new LoopStat(suite_info.num_loop_lengths);

    writeln("\n computeReferenceLoopTimes...");

    lstat0 = ref_loop_stat; // ???

    for ilen in 0..#LoopLength.NUM_LENGTHS {
      runReferenceLoop0(lstat0, ilen);
    }

    var lstat1 = new LoopStat(suite_info.num_loop_lengths);
    lstat1 = ref_loop_stat; // ???

    for ilen in 0..#LoopLength.NUM_LENGTHS {
      runReferenceLoop1(lstat1, ilen);
    }

    for ilen in 0..#LoopLength.NUM_LENGTHS {
      ref_loop_stat.loop_run_time[ilen].push_back(
        min(lstat0.loop_run_time[ilen][0], lstat1.loop_run_time[ilen][0]));
    }
  }

  proc defineReferenceLoopRunInfo() {
    var suite_info = getLoopSuiteRunInfo();
    var ref_loop_stat = new LoopStat(LoopLength.NUM_LENGTHS);
    suite_info.ref_loop_stat = ref_loop_stat;

    ref_loop_stat.loop_length[LoopLength.LONG]        = 24336;
    ref_loop_stat.loop_length[LoopLength.MEDIUM]      = 3844;
    ref_loop_stat.loop_length[LoopLength.SHORT]       = 64;
    ref_loop_stat.samples_per_pass[LoopLength.LONG]   = 30000;
    ref_loop_stat.samples_per_pass[LoopLength.MEDIUM] = 300000;
    ref_loop_stat.samples_per_pass[LoopLength.SHORT]  = 50000000;
  }

  proc defineLoopSuiteRunInfo(run_variants, run_loop,
                              sample_frac:real, loop_length_factor:real) {

    var suite_info = getLoopSuiteRunInfo();
    var run_variant_names = getVariantNames(run_variants);

    writeln("\n defineLoopSuiteRunInfo...");

    suite_info.loop_samp_frac = sample_frac;

    suite_info.loop_weights.resize(WeightGroup.NUM_WEIGHT_GROUPS);
    suite_info.loop_weights[WeightGroup.DATA_PARALLEL] = 2.0;
    suite_info.loop_weights[WeightGroup.ORDER_DEPENDENT] = 1.8;
    suite_info.loop_weights[WeightGroup.TRANSCENDENTAL] = 1.7;
    suite_info.loop_weights[WeightGroup.DATA_DEPENDENT] = 1.7;
    suite_info.loop_weights[WeightGroup.POINTER_NEST] = 1.4;
    suite_info.loop_weights[WeightGroup.COMPLEX] = 1.0;

    suite_info.loop_length_names.resize(LoopLength.NUM_LENGTHS);
    suite_info.loop_length_names[LoopLength.LONG] = "LONG";
    suite_info.loop_length_names[LoopLength.MEDIUM] = "MEDIUM";
    suite_info.loop_length_names[LoopLength.SHORT] = "SHORT";

    suite_info.num_loops_run.resize(run_variant_names.size());
    suite_info.tot_time.resize(run_variant_names.size());
    suite_info.fom_rel.resize(run_variant_names.size());
    suite_info.fom_rate.resize(run_variant_names.size());

    for ilv in 0..#run_variant_names.size() {
      suite_info.addLoopStats(run_variant_names[ilv]);

      if suite_info.num_loops_run[ilv] == nil then
        suite_info.num_loops_run[ilv] = new vector(int);
      suite_info.num_loops_run[ilv].resize(LoopLength.NUM_LENGTHS);

      if suite_info.tot_time[ilv] == nil then
        suite_info.tot_time[ilv] = new vector(real);
      suite_info.tot_time[ilv].resize(LoopLength.NUM_LENGTHS);

      if suite_info.fom_rel[ilv] == nil then
        suite_info.fom_rel[ilv] = new vector(real);
      suite_info.fom_rel[ilv].resize(LoopLength.NUM_LENGTHS);

      if suite_info.fom_rate[ilv] == nil then
        suite_info.fom_rate[ilv] = new vector(real);
      suite_info.fom_rate[ilv].resize(LoopLength.NUM_LENGTHS);
    }

    var shared_loop_length = new vector(int);
    shared_loop_length.resize(LoopLength.NUM_LENGTHS);
    shared_loop_length[LoopLength.LONG]   = (44217 * loop_length_factor):int;
    shared_loop_length[LoopLength.MEDIUM] = (5001 * loop_length_factor):int;
    shared_loop_length[LoopLength.SHORT]  = (171 * loop_length_factor):int;

    // ADomain::loop_length_factor = loop_length_factor;
    var weight: vector(real) = suite_info.loop_weights;
    var max_loop_length: int = 0;

    for iloop in 0..#suite_info.num_loops {
      var loop_name = (iloop:LoopKernelID):string;
      var loop_stat = new LoopStat(suite_info.num_loop_lengths);
      var max_loop_indx = 0;
      if run_loop[iloop] {
        select iloop {
          when LoopKernelID.REF_LOOP {
            // no op
          }
          when LoopKernelID.PRESSURE_CALC {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];
            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];
            loop_stat.samples_per_pass[LoopLength.LONG]   = 15000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 200000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 10000000;
          }
          when LoopKernelID.PRESSURE_CALC_ALT {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];
            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];
            loop_stat.samples_per_pass[LoopLength.LONG]   = 15000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 200000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 10000000;
          }
          when LoopKernelID.ENERGY_CALC {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];
            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];
            loop_stat.samples_per_pass[LoopLength.LONG]   = 3000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 30000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;

          }
          when LoopKernelID.ENERGY_CALC_ALT {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];
            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];
            loop_stat.samples_per_pass[LoopLength.LONG]   = 3000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 30000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;
          }
          when LoopKernelID.VOL3D_CALC {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];

            const ndims = 3;

            var Ldomain = new ADomain(LoopLength.LONG, ndims);
            loop_stat.loop_length[LoopLength.LONG]   = Ldomain.lpz - Ldomain.fpz + 1;
            var Mdomain = new ADomain(LoopLength.MEDIUM, ndims);
            loop_stat.loop_length[LoopLength.MEDIUM] = Mdomain.lpz - Mdomain.fpz + 1;
            var Sdomain = new ADomain(LoopLength.SHORT, ndims);
            loop_stat.loop_length[LoopLength.SHORT]  = Sdomain.lpz - Sdomain.fpz + 1;

            max_loop_indx = Ldomain.lpn;

            loop_stat.samples_per_pass[LoopLength.LONG]   = 6500;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 30000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 800000;
          }
          when LoopKernelID.DEL_DOT_VEC_2D {
            loop_stat.loop_weight = weight[WeightGroup.DATA_PARALLEL];

            const ndims = 2;

            var Ldomain = new ADomain(LoopLength.LONG, ndims);
            loop_stat.loop_length[LoopLength.LONG]   = Ldomain.n_real_zones;
            var Mdomain = new ADomain(LoopLength.MEDIUM, ndims);
            loop_stat.loop_length[LoopLength.MEDIUM] = Mdomain.n_real_zones;
            var Sdomain = new ADomain(LoopLength.SHORT, ndims);
            loop_stat.loop_length[LoopLength.SHORT]  = Sdomain.n_real_zones;

            max_loop_indx = Ldomain.lrn;

            loop_stat.samples_per_pass[LoopLength.LONG]   = 4000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 25000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 2000000;
          }
          when LoopKernelID.COUPLE {
            loop_stat.loop_weight = weight[WeightGroup.TRANSCENDENTAL];

            const ndims = 3;

            var Ldomain = new ADomain(LoopLength.LONG, ndims);
            loop_stat.loop_length[LoopLength.LONG]   = Ldomain.lpz - Ldomain.fpz + 1;
            var Mdomain = new ADomain(LoopLength.MEDIUM, ndims);
            loop_stat.loop_length[LoopLength.MEDIUM] = Mdomain.lpz - Mdomain.fpz + 1;
            var Sdomain = new ADomain(LoopLength.SHORT, ndims);
            loop_stat.loop_length[LoopLength.SHORT]  = Sdomain.lpz - Sdomain.fpz + 1;

            max_loop_indx = Ldomain.lrn;

            loop_stat.samples_per_pass[LoopLength.LONG]   = 2000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 10000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 600000;
          }
          when LoopKernelID.FIR {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 10000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 80000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 3000000;
          }
          when LoopKernelID.INIT3 {
            loop_stat.loop_weight = weight[WeightGroup.DATA_PARALLEL];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 10000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 110000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 12000000;
          }
          when LoopKernelID.MULADDSUB {
            loop_stat.loop_weight = weight[WeightGroup.DATA_PARALLEL];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 12000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 140000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 15000000;
          }
          when LoopKernelID.IF_QUAD {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 3000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 30000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;
          }
          when LoopKernelID.TRAP_INT {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 4000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 32000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;
          }
          when LoopKernelID.HYDRO_1D {
            loop_stat.loop_weight = weight[WeightGroup.DATA_PARALLEL];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 30000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 320000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 15000000;
          }
          when LoopKernelID.ICCG {
            loop_stat.loop_weight = weight[WeightGroup.COMPLEX];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 20000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 200000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 6000000;
          }
          when LoopKernelID.INNER_PROD {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 50000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 600000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 30000000;
          }
          when LoopKernelID.BAND_LIN_EQ {
            loop_stat.loop_weight = weight[WeightGroup.COMPLEX];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 40000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 600000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 20000000;
          }
          when LoopKernelID.TRIDIAG_ELIM {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 10000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 100000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 3000000;
          }
          when LoopKernelID.EOS {
            loop_stat.loop_weight = weight[WeightGroup.DATA_PARALLEL];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 18000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 140000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 5000000;
          }
          when LoopKernelID.ADI {
            loop_stat.loop_weight = weight[WeightGroup.COMPLEX];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 1000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 9000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 300000;
          }
          when LoopKernelID.INT_PREDICT {
            loop_stat.loop_weight = weight[WeightGroup.POINTER_NEST];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 3000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 30000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 2000000;
          }
          when LoopKernelID.DIFF_PREDICT {
            loop_stat.loop_weight = weight[WeightGroup.POINTER_NEST];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 2000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 22000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1800000;
          }
          when LoopKernelID.FIRST_SUM {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 30000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 250000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 8000000;
          }
          when LoopKernelID.FIRST_DIFF {
            loop_stat.loop_weight = weight[WeightGroup.DATA_PARALLEL];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 30000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 500000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 30000000;
          }
          when LoopKernelID.PIC_2D {
            loop_stat.loop_weight = weight[WeightGroup.COMPLEX];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 2000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 18000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 700000;
          }
          when LoopKernelID.PIC_1D {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 3000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 24000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;
          }
          when LoopKernelID.HYDRO_2D {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 300;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 2000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 50000;
          }
          when LoopKernelID.GEN_LIN_RECUR {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 4000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 36000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;
          }
          when LoopKernelID.DISC_ORD {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 1000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 8000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 200000;
          }
          when LoopKernelID.MAT_X_MAT {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 8;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 70;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 8000;
          }
          when LoopKernelID.PLANCKIAN {
            loop_stat.loop_weight = weight[WeightGroup.TRANSCENDENTAL];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 4000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 30000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;
          }
          when LoopKernelID.IMP_HYDRO_2D {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 800;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 6000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 150000;
          }
          when LoopKernelID.FIND_FIRST_MIN {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];

            for i in 0..#LoopLength.NUM_LENGTHS {
              loop_stat.loop_length[i] = shared_loop_length[i];
            }
            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 50000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 330000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 8000000;
          }
          otherwise {
            halt("Unknown loop id: ", iloop);
          }
        }
      }
      suite_info.loop_names.push_back(loop_name);
      max_loop_length = max(max_loop_length, max_loop_indx);

      for i in 0..#suite_info.num_loop_lengths {
        loop_stat.samples_per_pass[i] =
          (loop_stat.samples_per_pass[i] * suite_info.loop_samp_frac /
           loop_length_factor):int;
        if suite_info.run_loop_length[i] {
          loop_stat.loop_run_count[i] =
            loop_stat.samples_per_pass[i] * suite_info.num_suite_passes;
        } else {
          loop_stat.loop_run_count[i] = 0;
        }
      }
      for ilv in 0..#run_variant_names.size() {
        suite_info.getLoopStats(run_variant_names[ilv]).push_back(loop_stat);
      }
    }
    defineReferenceLoopRunInfo();
    s_loop_data = new LoopData(max(max_loop_length, suite_info.ref_loop_stat.loop_length[LoopLength.LONG]));
  }
}
