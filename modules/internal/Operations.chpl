module Operations {

  class OperationsHandler {
    proc processOperations( size:int, buf:c_void_ptr ) {
      halt("virtual OperationHandler.processOperations called");
    }
  }

  export proc chpl_process_operations(obj:c_void_ptr, size:int, buf:c_void_ptr)
  {
    var x = obj:OperationsHandler;
    x.processOperations(size, buf);
  }

  // Should this be a method in OperationsHandler ?
  // buf can be reused immediately after this call
  proc enqueue_operation(obj:OperationsHandler, mode:int(32), idx:int, size:int, buf:c_void_ptr) {

    extern proc chpl_gen_comm_start_op(node:int(32), subloc:int(32),
                                       fid:int(32),
                                       obj:c_void_ptr, args:c_void_ptr,
                                       size:size_t, mode:int(32), idx:int);

    var loc = __primitive("_wide_get_locale", obj);
    var node = chpl_nodeFromLocaleID(loc);
    var subloc = chpl_sublocFromLocaleID(loc);
    var raddr = __primitive("_wide_get_addr", obj);
    chpl_gen_comm_start_op(node, subloc, -1, raddr, buf, size:size_t,
                           mode, idx);
  }

  proc flush_operations(obj:OperationsHandler, mode:int(32), idx:int) {

    extern proc chpl_gen_comm_finish_ops(node:int(32), subloc:int(32),
                                       fid:int(32),
                                       obj:c_void_ptr, mode:int(32), idx:int);

    var loc = __primitive("_wide_get_locale", obj);
    var node = chpl_nodeFromLocaleID(loc);
    var subloc = chpl_sublocFromLocaleID(loc);
    var raddr = __primitive("_wide_get_addr", obj);
    chpl_gen_comm_finish_ops(node, subloc, -1, raddr, mode, idx);
  }
}
