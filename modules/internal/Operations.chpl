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

}
