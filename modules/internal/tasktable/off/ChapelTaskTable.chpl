// ChapelTaskTable.chpl
//
module ChapelTaskTable {

  ////////////////////////////////////////////////////////////////////////{
  //
  // Exported task table code.
  //
  // In the --no-task-table mode, these are just no-op stubs and a halt()
  // if someone tries to print the task table.
  //
  ////////////////////////////////////////////////////////////////////////}

  export proc chpldev_taskTable_add(taskID   : chpl_taskID_t,
                                    lineno   : uint(32),
                                    filename : c_string,
                                    tl_info  : uint(64))
  {
  }

  export proc chpldev_taskTable_remove(taskID : chpl_taskID_t)
  {
  }

  export proc chpldev_taskTable_set_active(taskID : chpl_taskID_t)
  {
  }

  export proc chpldev_taskTable_set_suspended(taskID : chpl_taskID_t)
  {
  }

  export proc chpldev_taskTable_get_tl_info(taskID : chpl_taskID_t)
  {
  }

  export proc chpldev_taskTable_print()
  {
    halt("To use task tracking, you must recompile with --task-tracking");
  }
}
