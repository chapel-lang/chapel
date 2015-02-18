module LCALSStatic {
  use LCALSDataTypes;

  config const do_fom = true;
  config const run_misc = false;
  config const output_dirname: string = "lcalsOutput";

  var s_loop_data: LoopData;
  var s_loop_suite_run_info = new LoopSuiteRunInfo();

  proc getLoopSuiteRunInfo() {
    return s_loop_suite_run_info;
  }

  proc getLoopData() {
    return s_loop_data;
  }
}
