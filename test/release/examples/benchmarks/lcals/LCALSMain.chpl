use FileSystem;
use LCALSParams;
use LCALSEnums;
use LCALSDataTypes;
use LCALSLoops;
use LCALSStatic;
use LCALSConfiguration;
use LCALSChecksums;

use RunARawLoops;
use RunBRawLoops;
use RunCRawLoops;
use RunParallelRawLoops;
use RunSPMDRawLoops;
use RunVectorizeOnlyRawLoops;

use IO;

proc allocateLoopData() {
/*  Currently unused:
  const num_aligned_segments =
    (s_loop_data!.max_loop_length + 20)/LCALS_DATA_ALIGN + 1;
  const aligned_chunksize = num_aligned_segments*LCALS_DATA_ALIGN;
*/
  writeln("\n allocateLoopData...");

  for i in 0..#s_loop_data!.s_num_1D_Real_arrays {
    initData(s_loop_data!.RealArray_1D[i], i+1);
  }
  for i in 0..#s_loop_data!.s_num_1D_Nx4_Real_arrays {
    initData(s_loop_data!.RealArray_1D_Nx4[i], i+1);
  }
  for i in 0..#s_loop_data!.s_num_1D_Indx_arrays {
    initData(s_loop_data!.IndxArray_1D[i], i+1);
  }
  for i in 0..#s_loop_data!.s_num_1D_Complex_arrays {
    initData(s_loop_data!.ComplexArray_1D[i], i+1);
  }
  for i in 0..#s_loop_data!.s_num_2D_Nx25_Real_arrays {
    initData(s_loop_data!.RealArray_2D_Nx25[i], i+1);
  }
  for i in 0..#s_loop_data!.s_num_2D_7xN_Real_arrays {
    initData(s_loop_data!.RealArray_2D_7xN[i], i+1);
  }
  for i in 0..#s_loop_data!.s_num_2D_64x64_Real_arrays {
    initData(s_loop_data!.RealArray_2D_64x64[i], i+1);
  }
  for i in 0..#s_loop_data!.s_num_3D_2xNx4_Real_arrays {
    initData(s_loop_data!.RealArray_3D_2xNx4[i], i+1);
  }
}


proc main {
  const LoopKernelDom = {LoopKernelID.REF_LOOP..LoopKernelID.FIND_FIRST_MIN};
  const LoopLengthDom = {LoopLength.LONG..LoopLength.SHORT};
  const LoopVariantIDDom = {LoopVariantID.RAW..LoopVariantID.FORALL_HYBRID_LAMBDA_TYPEFIX};

  const sample_frac = 1.0;
  const loop_length_factor = 1.0;
  const run_loop_length: [LoopLengthDom] bool =
    (run_longLoops, run_mediumLoops, run_shortLoops);
  var run_loop: [LoopKernelDom] bool;
  var run_variants: [LoopVariantIDDom] bool;

  run_loop[LoopKernelID.REF_LOOP] = run_refLoop;

  // Loop Subset A: Loops extracted from LLNL app codes.
  run_loop[LoopKernelID.PRESSURE_CALC ] = runA_pressureCalc;
  run_loop[LoopKernelID.ENERGY_CALC   ] = runA_energyCalc;
  run_loop[LoopKernelID.VOL3D_CALC    ] = runA_vol3DCalc;
  run_loop[LoopKernelID.DEL_DOT_VEC_2D] = runA_delDotVec2D;
  run_loop[LoopKernelID.COUPLE        ] = runA_couple;
  run_loop[LoopKernelID.FIR           ] = runA_fir;

  // Loop Subset B: "Basic" Loops.
  run_loop[LoopKernelID.INIT3         ] = runB_init3;
  run_loop[LoopKernelID.MULADDSUB     ] = runB_muladdsub;
  run_loop[LoopKernelID.IF_QUAD       ] = runB_ifQuad;
  run_loop[LoopKernelID.TRAP_INT      ] = runB_trapInt;

  // Loop Subset C: Loops from older Livermore Loops in "C" suite.
  run_loop[LoopKernelID.HYDRO_1D      ] = runC_hydro1D;
  run_loop[LoopKernelID.ICCG          ] = runC_iccg;
  run_loop[LoopKernelID.INNER_PROD    ] = runC_innerProd;
  run_loop[LoopKernelID.BAND_LIN_EQ   ] = runC_bandLinEq;
  run_loop[LoopKernelID.TRIDIAG_ELIM  ] = runC_tridiagElim;
  run_loop[LoopKernelID.EOS           ] = runC_eos;
  run_loop[LoopKernelID.ADI           ] = runC_adi;
  run_loop[LoopKernelID.INT_PREDICT   ] = runC_intPredict;
  run_loop[LoopKernelID.DIFF_PREDICT  ] = runC_diffPredict;
  run_loop[LoopKernelID.FIRST_SUM     ] = runC_firstSum;
  run_loop[LoopKernelID.FIRST_DIFF    ] = runC_firstDiff;
  run_loop[LoopKernelID.PIC_2D        ] = runC_pic2D;
  run_loop[LoopKernelID.PIC_1D        ] = runC_pic1D;
  run_loop[LoopKernelID.HYDRO_2D      ] = runC_hydro2D;
  run_loop[LoopKernelID.GEN_LIN_RECUR ] = runC_genLinRecur;
  run_loop[LoopKernelID.DISC_ORD      ] = runC_discOrd;
  run_loop[LoopKernelID.MAT_X_MAT     ] = runC_matXMat;
  run_loop[LoopKernelID.PLANCKIAN     ] = runC_planckian;
  run_loop[LoopKernelID.IMP_HYDRO_2D  ] = runC_impHydro2D;
  run_loop[LoopKernelID.FIND_FIRST_MIN] = runC_findFirstMin;

  if output_dirname.size > 0 {
    mkdir(output_dirname, parents=true);
  }

  if run_variantRaw then
    run_variants[LoopVariantID.RAW] = true;
  if run_variantRawOmp then
    run_variants[LoopVariantID.RAW_OMP] = true;
  if run_variantRawSPMD {
    if run_mediumLoops || run_shortLoops {
      // Exit early so the benchmark doesn't run for a long time before
      // reaching this same halt in runSPMDRawLoops
      halt("SHORT and MEDIUM loop lengths are currently disabled " +
           "for this variant because they take too long to execute");
    }
    run_variants[LoopVariantID.RAW_SPMD] = true;
  }
  if run_variantRawVectorizeOnly then
    run_variants[LoopVariantID.RAW_VECTOR_ONLY] = true;


  assert(!run_variantForallLambda &&
         !run_variantForallLambdaOmp);
  /*
  if run_variantForallLambda then
    run_variants[LoopVariantID.FORALL_LAMBDA] = true;
  if run_variantForallLambdaOmp then
    run_variants[LoopVariantID.FORALL_LAMBDA_OMP] = true;
  */

  const cache_size = 0;
  const host_name = here.name;
  if outputFormat != OutputStyle.MINIMAL then
    writeln("\n Running loop suite on ", host_name);

  allocateLoopSuiteRunInfo(host_name,
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
    for variant in run_variants.domain do if run_variants[variant] {
      const loop_variant_name = getVariantName(variant);
      writeln("\t run loop variant ---> ", loop_variant_name);
      for ilen in LoopLengthDom {
        if run_loop_length[ilen] {
          runLoopVariant(variant, run_loop, ilen);
        }
      }
    }
  }

  writeln("\n generate reports....");

  select outputFormat {
    when OutputStyle.MINIMAL {
      // no timing or checksum output
    }
    when OutputStyle.REFERENCE {
      // match what the reference code does
      generateTimingReport(run_variants, output_dirname);
      generateChecksumReport(run_variants, output_dirname);
      generateFOMReport(run_variants, output_dirname);
    }
    when OutputStyle.PERF_TEST {
      // format times for nightly perf testing
      generatePerfTimingReport(run_variants);
    }
    otherwise {
      writeln("Unexpected output format: ", outputFormat);
    }
  }

  if verify_checksums || verify_checksums_abbreviated {
    checkChecksums(run_variants, run_loop, run_loop_length);
  }

  writeln("\n freeLoopSuiteRunInfo...");
  writeln("\n DONE!!! ");
}

proc computeStats(ilv: LoopVariantID, loop_stats: [] shared LoopStat, do_fom: bool) {
  for stat in loop_stats {
    for ilen in stat.loop_length_dom {
      if stat.loop_run_count[ilen] > 0 {
        var time_sample = stat.loop_run_time[ilen].borrow();
        var sample_size = time_sample.size;
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

/* Print timing data in a format that is easier to automatically process
   with our performance testing harness vs. the regular LCALS output.
 */
proc generatePerfTimingReport(loop_variants: [] bool) {
  if + reduce loop_variants == 0 then return;
  const ver_info = buildVersionInfo();
  var suite_run_info = getLoopSuiteRunInfo();
  const nvariants = loop_variants.size;
  for ilv in loop_variants.domain {
    computeStats(ilv, suite_run_info.getLoopStats(ilv), do_fom);
  }
  for iloop in suite_run_info.loop_kernel_dom {
    for variant in loop_variants.domain {
      var stat = suite_run_info.getLoopStats(variant)[iloop].borrow();
      if stat.loop_is_run {
        for ilen in stat.loop_length_dom {
          if stat.loop_run_count[ilen] > 0 {
            var strVar = (variant:string).toLower();
            var strKer = (iloop:string).toLower();
            var strLen = (ilen:string).toLower();
            writeln(strVar, "_", strKer, "_", strLen, " ", stat.mean[ilen]);
          }
        }
      }
    }
  }
}

proc generateTimingReport(loop_variants: [] bool,
                          output_dirname: string) {
  if + reduce loop_variants == 0 then return;
  const ver_info = buildVersionInfo();
  var suite_run_info = getLoopSuiteRunInfo();
  const nvariants = loop_variants.size;
  for ilv in loop_variants.domain {
    computeStats(ilv, suite_run_info.getLoopStats(ilv), do_fom);
  }
  if output_dirname.size != 0 {
    const timing_fname = output_dirname + "/" + "timing.txt";
    var outfile = open(timing_fname, iomode.cw);
    var writer = outfile.writer();

    writeTimingSummaryReport(loop_variants, writer);

    writer.close();
    outfile.close();

    for ilv in loop_variants.domain {
      writeMeanTimeReport(ilv, output_dirname);
    }
    for ilv in loop_variants.domain {
      // FIXME: I think this should skip the lowest index (the reference)
      writeRelativeTimeReport(ilv, output_dirname);
    }

  } else {
    writeTimingSummaryReport(loop_variants, stdout);
  }
}

proc writeMeanTimeReport(variant: LoopVariantID, output_dirname: string) {
  const variant_name = getVariantName(variant);
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

  for i in 0..#len_names.size {
    writer.write(sepchr);
  }
  writer.writeln();

  for name in len_names {
    writer.write(sepchr, name);
  }
  writer.writeln();

  for iloop in suite_run_info.loop_kernel_dom {
    var stat = suite_run_info.getLoopStats(variant)[iloop].borrow();
    if loop_names[iloop].size != 0 && stat.loop_is_run {
      writer.write(loop_names[iloop]);
      for ilen in stat.loop_length_dom {
        writer.writef("%s%r", sepchr, stat.mean[ilen]);
      }
      writer.writeln();
    }
  }
  writer.close();
  outfile.close();
}

proc writeRelativeTimeReport(variant: LoopVariantID, output_dirname: string) {
  const variant_name = getVariantName(variant);
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
  for i in 0..#len_names.size {
    writer.write(sepchr);
  }
  writer.writeln();

  for name in len_names {
    writer.write(sepchr, name);
  }
  writer.writeln();

  for iloop in suite_run_info.loop_kernel_dom {
    var stat = suite_run_info.getLoopStats(variant)[iloop].borrow();
    if loop_names[iloop].size != 0 && stat.loop_is_run {
      writer.write(loop_names[iloop]);
      for ilen in stat.loop_length_dom {
        writer.write(sepchr, stat.meanrel2ref[ilen]);
      }
      writer.writeln();
    }
  }
  writer.close();
  outfile.close();
}

proc writeTimingSummaryReport(loop_variants: [] bool, outchannel) {
  var suite_run_info = getLoopSuiteRunInfo();
  const nvariants = loop_variants.size;
  const loop_variant_names = getVariantNames(loop_variants);
  const ref_variant_name = loop_variant_names[1:LoopVariantID];
  var loop_names = suite_run_info.loop_names;

  const equal_line = "===========================================================================================================\n";
  const dash_line = "------------------------------------------------------------------------------------------------------------\n";
  const dash_line_part = "-------------------------------------------------------\n";
  const dot_line_part = "............................................\n";
  var len_id: [suite_run_info.loop_length_dom] string;
  for ilen in suite_run_info.loop_length_dom {
    len_id[ilen] = suite_run_info.loop_length_names[ilen][1];
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
  outchannel.writeln("sizeof(Real_type) = ", numBytes(real));
  outchannel.writeln("     num suite passes = ", suite_run_info.num_suite_passes);
  outchannel.writeln("     loop sample fraction = ", suite_run_info.loop_samp_frac);
  outchannel.write("     loop variants run : ");
  var var_num = 0;
  const num_variants_run = + reduce loop_variants;
  for variant in loop_variants.domain do if loop_variants[variant] {
    var last_char: string;
    var_num += 1;
    if ( var_num < num_variants_run ) then last_char = " , ";
    outchannel.write(loop_variant_names[variant], last_char);
  }
  outchannel.writeln("\n     reference variant : ", ref_variant_name);
  outchannel.write(equal_line);
  outchannel.write(equal_line);

  var max_name_len = 0;
  for name in loop_names {
    max_name_len = max(max_name_len, name.size);
  }
  var max_var_name_len = 0;
  for ilv in loop_variants.domain {
    max_var_name_len = max(max_var_name_len, loop_variant_names[ilv].size);
  }
  var var_field = "Variant(length id)";
  var var_field_len = var_field.size;
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

  for iloop in suite_run_info.loop_kernel_dom {
    var ref_variant_stat = suite_run_info.getLoopStats(LoopVariantID.RAW)[iloop].borrow();

    var ref_mean = ref_variant_stat.mean;

    if loop_names[iloop].size != 0 && ref_variant_stat.loop_is_run {
      if iloop:int > 1 {
        outchannel.write("\n", dash_line_part);
      }
      outchannel.writef("%s (%i) --> ", loop_names[iloop], iloop);
      for (ilv, variant) in zip(0..#nvariants, loop_variants.domain) {
        var stat = suite_run_info.getLoopStats(variant)[iloop].borrow();
        if stat.loop_is_run {
          if ilv == 0 {
            for ilen in stat.loop_length_dom {
              outchannel.writef("   %s:(%i, %i)", len_id[ilen], stat.loop_length[ilen], stat.samples_per_pass[ilen]);
            }
            outchannel.writeln();
          } else {
            outchannel.write(dot_line_part);
          }
          for ilen in stat.loop_length_dom {
            if stat.loop_run_count[ilen] > 0 {
              var var_string = loop_variant_names[variant] + "(" + len_id[ilen] + ")";
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
                outchannel.writef("%*s\n", prec_buf, rel_mean_diff);
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

proc generateChecksumReport(loop_variants: [] bool,
                            output_dirname: string) {
  if + reduce loop_variants == 0 then return;
  if output_dirname.size != 0 {
    var checksum_fname = output_dirname + "/" + "checksum.txt";
    var outfile = open(checksum_fname, iomode.cw);
    var writer = outfile.writer();
    writer.write("\n writeChecksumReport...    ", checksum_fname);
    writeChecksumReport(loop_variants, writer);
    writer.close();
    outfile.close();
  } else {
    writeChecksumReport(loop_variants, stdout);
  }
}

proc writeChecksumReport(loop_variants: [] bool, outchannel) {
  var suite_run_info = getLoopSuiteRunInfo();
  var nvariants = loop_variants.size;
  const loop_variant_names = getVariantNames(loop_variants);
  var loop_names = suite_run_info.loop_names;
  var len_id: [suite_run_info.loop_length_dom] string;

  for ilen in suite_run_info.loop_length_dom {
    len_id[ilen] = suite_run_info.loop_length_names[ilen][1];
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
  for name in loop_names {
    max_name_len = max(max_name_len, name.size);
  }
  var max_var_name_len = 0;
  for ilv in loop_variant_names.domain {
    max_var_name_len = max(max_var_name_len, loop_variant_names[ilv].size);
  }

  var var_field = "Variant(length id)";
  var var_field_len = max(var_field.size, max reduce [ilv in loop_variant_names.domain]  loop_variant_names[ilv].size + 3);
  var prec = 32;
  var prec_buf = prec + 8;

  outchannel.writeln("Loop name -->");
  outchannel.writef("%-*s", var_field_len+1, var_field);
  outchannel.writef("%*s", prec_buf, "Check Sum    ");
  outchannel.writef("%-*s\n", prec_buf, "        Delta from reference");
  outchannel.write(dash_line);


  for iloop in suite_run_info.loop_kernel_dom {
    var ref_variant_stat = suite_run_info.getLoopStats(LoopVariantID.RAW)[iloop].borrow();
    var ref_chksum = ref_variant_stat.loop_chksum;
    if loop_names[iloop].size != 0 && ref_variant_stat.loop_is_run {
      if iloop:int > 1 then
        outchannel.write("\n", dash_line_part);
      outchannel.write(loop_names[iloop], "(", iloop:int, ") --> ");
      for (ilv, variant) in zip(0..#nvariants, loop_variant_names.domain) {
        var stat = suite_run_info.getLoopStats(variant)[iloop].borrow();
        if stat.loop_is_run {

          if ilv == 0 then outchannel.writeln();
          else outchannel.write(dot_line_part);

          for ilen in stat.loop_length_dom {
            if stat.loop_run_count[ilen] > 0 {
              var var_string = loop_variant_names[variant] + "(" + len_id[ilen] + ")";
              outchannel.writef("%-*s", var_field_len+1, var_string);
              outchannel.writef("%{#########.#######################}", stat.loop_chksum[ilen]);
              if ilv > 0 {
                var chksum_diff = abs(stat.loop_chksum[ilen] - ref_chksum[ilen]);
                outchannel.writef("%*r\n", prec_buf, chksum_diff);
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

proc generateFOMReport(loop_variants: [] bool,
                       output_dirname: string) {
}

proc runLoopVariant(lvid: LoopVariantID, run_loop:[] bool, ilength: LoopLength) {
  var loop_suite_run_info = getLoopSuiteRunInfo();
  ref loop_stats = loop_suite_run_info.getLoopStats(lvid);
  select lvid {
    when LoopVariantID.RAW {
      runARawLoops(loop_stats, run_loop, ilength);
      runBRawLoops(loop_stats, run_loop, ilength);
      runCRawLoops(loop_stats, run_loop, ilength);
    }
    when LoopVariantID.RAW_OMP {
      runParallelRawLoops(loop_stats, run_loop, ilength);
    }
    when LoopVariantID.RAW_SPMD {
      runSPMDRawLoops(loop_stats, run_loop, ilength);
    }
    when LoopVariantID.RAW_VECTOR_ONLY {
      runVectorizeOnlyRawLoops(loop_stats, run_loop, ilength);
    }
/*
    when LoopVariantID.FORALL_LAMBDA {
      runAForallLambdaLoops(loop_stats, run_loop, ilength);
      runBForallLambdaLoops(loop_stats, run_loop, ilength);
      runCForallLambdaLoops(loop_stats, run_loop, ilength);
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
                              num_suite_passes: int,
                              run_loop_length: [] bool,
                              cache_size: int) {
  extern proc drand48(): real;
  writeln("\n allocateLoopSuiteRunInfo...");

  s_loop_suite_run_info.host_name = host_name;
  s_loop_suite_run_info.num_suite_passes = num_suite_passes;
  s_loop_suite_run_info.run_loop_length = run_loop_length;
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

proc getVariantNames(lvids: [] bool) {
  var run_variant_names: [lvids.domain] string;
  for lvid in lvids.domain {
    run_variant_names[lvid] = getVariantName(lvid);
  }
  return run_variant_names;
}

proc computeReferenceLoopTimes() {
  var suite_info = getLoopSuiteRunInfo();
  var ref_loop_stat: LoopStat = suite_info.ref_loop_stat.borrow()!;

  writeln("\n computeReferenceLoopTimes...");

  var lstat0 = ref_loop_stat;

  for ilen in suite_info.loop_length_dom {
    runReferenceLoop0(lstat0, ilen);
  }

  var lstat1 = ref_loop_stat;

  for ilen in suite_info.loop_length_dom {
    runReferenceLoop1(lstat1, ilen);
  }

  for ilen in suite_info.loop_length_dom {
    ref_loop_stat.loop_run_time[ilen].push_back(
      min(lstat0.loop_run_time[ilen][0], lstat1.loop_run_time[ilen][0]));
  }
}

proc defineReferenceLoopRunInfo() {
  var suite_info = getLoopSuiteRunInfo();
  const ref_loop_stat = suite_info.ref_loop_stat.borrow();

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
  const num_lengths = suite_info.loop_length_dom.size;

  writeln("\n defineLoopSuiteRunInfo...");

  suite_info.loop_samp_frac = sample_frac;

  suite_info.loop_weights[WeightGroup.DATA_PARALLEL] = 2.0;
  suite_info.loop_weights[WeightGroup.ORDER_DEPENDENT] = 1.8;
  suite_info.loop_weights[WeightGroup.TRANSCENDENTAL] = 1.7;
  suite_info.loop_weights[WeightGroup.DATA_DEPENDENT] = 1.7;
  suite_info.loop_weights[WeightGroup.POINTER_NEST] = 1.4;
  suite_info.loop_weights[WeightGroup.COMPLEX] = 1.0;

  suite_info.loop_length_names[LoopLength.LONG] = "LONG";
  suite_info.loop_length_names[LoopLength.MEDIUM] = "MEDIUM";
  suite_info.loop_length_names[LoopLength.SHORT] = "SHORT";

  var shared_loop_length: [suite_info.loop_length_dom] int;
  shared_loop_length[LoopLength.LONG]   = (44217 * loop_length_factor):int;
  shared_loop_length[LoopLength.MEDIUM] = (5001 * loop_length_factor):int;
  shared_loop_length[LoopLength.SHORT]  = (171 * loop_length_factor):int;

  // ADomain::loop_length_factor = loop_length_factor;
  var weight = suite_info.loop_weights;
  var max_loop_length: int = 0;
  for ilv in run_variants.domain {
    for iloop in suite_info.loop_kernel_dom {
      var loop_name = iloop:string;
      var loop_stat = new shared LoopStat();
      var max_loop_indx = 0;
      if run_loop[iloop] {
        select iloop {
          when LoopKernelID.REF_LOOP {
            // no op
          }
          when LoopKernelID.PRESSURE_CALC {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];
            loop_stat.samples_per_pass[LoopLength.LONG]   = 15000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 200000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 10000000;
          }
          when LoopKernelID.PRESSURE_CALC_ALT {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];
            loop_stat.samples_per_pass[LoopLength.LONG]   = 15000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 200000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 10000000;
          }
          when LoopKernelID.ENERGY_CALC {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];
            loop_stat.samples_per_pass[LoopLength.LONG]   = 3000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 30000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;

          }
          when LoopKernelID.ENERGY_CALC_ALT {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

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
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 10000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 80000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 3000000;
          }
          when LoopKernelID.INIT3 {
            loop_stat.loop_weight = weight[WeightGroup.DATA_PARALLEL];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 10000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 110000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 12000000;
          }
          when LoopKernelID.MULADDSUB {
            loop_stat.loop_weight = weight[WeightGroup.DATA_PARALLEL];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 12000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 140000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 15000000;
          }
          when LoopKernelID.IF_QUAD {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 3000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 30000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;
          }
          when LoopKernelID.TRAP_INT {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 4000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 32000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;
          }
          when LoopKernelID.HYDRO_1D {
            loop_stat.loop_weight = weight[WeightGroup.DATA_PARALLEL];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 30000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 320000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 15000000;
          }
          when LoopKernelID.ICCG {
            loop_stat.loop_weight = weight[WeightGroup.COMPLEX];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 20000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 200000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 6000000;
          }
          when LoopKernelID.INNER_PROD {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 50000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 600000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 30000000;
          }
          when LoopKernelID.BAND_LIN_EQ {
            loop_stat.loop_weight = weight[WeightGroup.COMPLEX];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 40000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 600000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 20000000;
          }
          when LoopKernelID.TRIDIAG_ELIM {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 10000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 100000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 3000000;
          }
          when LoopKernelID.EOS {
            loop_stat.loop_weight = weight[WeightGroup.DATA_PARALLEL];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 18000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 140000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 5000000;
          }
          when LoopKernelID.ADI {
            loop_stat.loop_weight = weight[WeightGroup.COMPLEX];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 1000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 9000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 300000;
          }
          when LoopKernelID.INT_PREDICT {
            loop_stat.loop_weight = weight[WeightGroup.POINTER_NEST];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 3000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 30000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 2000000;
          }
          when LoopKernelID.DIFF_PREDICT {
            loop_stat.loop_weight = weight[WeightGroup.POINTER_NEST];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 2000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 22000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1800000;
          }
          when LoopKernelID.FIRST_SUM {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 30000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 250000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 8000000;
          }
          when LoopKernelID.FIRST_DIFF {
            loop_stat.loop_weight = weight[WeightGroup.DATA_PARALLEL];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 30000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 500000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 30000000;
          }
          when LoopKernelID.PIC_2D {
            loop_stat.loop_weight = weight[WeightGroup.COMPLEX];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 2000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 18000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 700000;
          }
          when LoopKernelID.PIC_1D {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 3000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 24000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;
          }
          when LoopKernelID.HYDRO_2D {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 300;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 2000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 50000;
          }
          when LoopKernelID.GEN_LIN_RECUR {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 4000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 36000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;
          }
          when LoopKernelID.DISC_ORD {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 1000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 8000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 200000;
          }
          when LoopKernelID.MAT_X_MAT {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 8;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 70;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 8000;
          }
          when LoopKernelID.PLANCKIAN {
            loop_stat.loop_weight = weight[WeightGroup.TRANSCENDENTAL];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 4000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 30000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 1000000;
          }
          when LoopKernelID.IMP_HYDRO_2D {
            loop_stat.loop_weight = weight[WeightGroup.ORDER_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 800;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 6000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 150000;
          }
          when LoopKernelID.FIND_FIRST_MIN {
            loop_stat.loop_weight = weight[WeightGroup.DATA_DEPENDENT];
            loop_stat.loop_length = shared_loop_length;

            max_loop_indx = loop_stat.loop_length[LoopLength.LONG];

            loop_stat.samples_per_pass[LoopLength.LONG]   = 50000;
            loop_stat.samples_per_pass[LoopLength.MEDIUM] = 330000;
            loop_stat.samples_per_pass[LoopLength.SHORT]  = 8000000;
          }
          otherwise {
            halt("Unknown loop id: ", iloop);
          }
        }

        if verify_checksums_abbreviated {
          loop_stat.samples_per_pass[LoopLength.LONG]   = 3;
          loop_stat.samples_per_pass[LoopLength.MEDIUM] = 3;
          loop_stat.samples_per_pass[LoopLength.SHORT]  = 3;
        }
      }

      suite_info.loop_names[iloop] = loop_name;
      max_loop_length = max(max_loop_length, max_loop_indx);

      for len in suite_info.loop_length_dom {
        loop_stat.samples_per_pass[len] =
          (loop_stat.samples_per_pass[len] * suite_info.loop_samp_frac /
           loop_length_factor):int;
        if suite_info.run_loop_length[len] {
          loop_stat.loop_run_count[len] =
            loop_stat.samples_per_pass[len] * suite_info.num_suite_passes;
        } else {
          loop_stat.loop_run_count[len] = 0;
        }
      }
      suite_info.getLoopStats(ilv)[iloop] = loop_stat;
    }
  }
  defineReferenceLoopRunInfo();
  s_loop_data = new LoopData(max(max_loop_length, suite_info.ref_loop_stat!.loop_length[LoopLength.LONG]));

}
