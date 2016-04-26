module LCALSStatic {
  use LCALSDataTypes;

  var s_loop_data: LoopData;
  var s_loop_suite_run_info = new LoopSuiteRunInfo();

  proc getLoopSuiteRunInfo() {
    return s_loop_suite_run_info;
  }

  proc getLoopData() {
    return s_loop_data;
  }
}
