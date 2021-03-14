module LCALSStatic {
  use LCALSDataTypes;

  var s_loop_data: owned LoopData?;
  var s_loop_suite_run_info = new LoopSuiteRunInfo();

  proc getLoopSuiteRunInfo() {
    return s_loop_suite_run_info.borrow();
  }

  proc getLoopData() {
    return s_loop_data.borrow()!;
  }
}
