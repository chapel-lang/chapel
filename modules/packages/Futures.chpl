module Futures {

  use Reflection;

  enum ExecMode {
    Async,
    Defer
  };

  class Future {
    var promise;
    var args;
    type retType = promise.retType;
    const execMode: ExecMode;
    var retval: sync retType;
    var launched: atomic bool;

    proc Future(promise, args...) {
      param mode = ExecMode.Async;
      if !canResolveMethod(promise, "this", (...args)) then
        compilerError("Type mismatch in function and arguments");
      execMode = mode;
      if mode == ExecMode.Defer then
        launched.write(false);
      else {
        launched.write(true);
        begin retval.writeEF(promise((...args)));
      }
    }

    proc Future(promise, args..., mode: ExecMode) {
      if !canResolveMethod(promise, "this", (...args)) then
        compilerError("Type mismatch in function and arguments");
      execMode = mode;
      if mode == ExecMode.Defer then
        launched.write(false);
      else {
        launched.write(true);
        begin retval.writeEF(promise((...args)));
      }
    }

    proc get(): retType {
      wait();
      return retval.readFF();
    }

    proc trigger() {
      if !launched.testAndSet() then
        begin retval.writeEF(promise((...args)));
    }

    proc wait() {
      if !launched.testAndSet() then
        retval.writeEF(promise((...args)));
    }

    proc ready(): bool {
      return retval.isFull;
    }

  } // class Future

} // module Futures
