module OuterModule {
  // extract from test/users/npadmana/mpi/ring.chpl
  // which uses MPI and Barriers (just once)

  use MPIvass;
  use BarriersVass;
  use BarriersVass2;

  proc main() {
    var sendBarrier = new Barrier(numLocales);
  }

  module MPIvass {
    proc Barrier(comm: int) { }
  }

  module BarriersVass {
    record Barrier {
      proc init(numTasks: int) { }
    }
  }

  module BarriersVass2 {
    record Barrier {
      proc init(numTasks: int) { }
    }
  }
}
