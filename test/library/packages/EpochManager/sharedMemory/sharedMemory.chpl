use EpochManager;

class C {
  var x : int;

  proc deinit() {
    writeln("Reclaimed.");
  }
}

proc main() {

  // Creating a manager instance
  var manager = new unmanaged EpochManager();

  // registering task with the manager
  forall i in 1..4 with (var tok = manager.register()) {
    // creating a new object
    var b = new unmanaged C(i);

    // pin the task
    tok.pin();

    // delete b
    tok.delete_obj(b);

    // unpin the task
    tok.unpin();
  }
  // attempt to reclaim memory
  manager.try_reclaim();

  delete manager;
}

