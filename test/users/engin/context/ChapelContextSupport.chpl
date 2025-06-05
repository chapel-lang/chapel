module ChapelContextSupport {

  pragma "context type"
  record Context {
    // TODO: we probably want different types for the loop's context and the
    // iterator's. Stuff here doesn't make much sense for the loop's context.
    // OTOH, we may want to use loop's context to set blockSize, for example.
    // Prototype challenge: PRIM_OUTER_CONTEXT's return type maybe a little
    // difficult to handle. But maybe return a c_ptr(void) and cast it?
    param rank: int;

    type idType = if rank==1 then int else rank*int;
    var taskId: idType;
    var numTasks: idType;

    proc init() {
      this.rank = 1;
      // this is a loop context and not an iterator context
    }

    proc init(param rank, taskId, numTasks) {
      this.rank=rank;
      this.taskId = taskId;
      this.numTasks = numTasks;
    }
  }
}
