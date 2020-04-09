/* A very basic MPI wrapper, to enable simple 
   interaction with MPI codes.

   Nikhil Padmanabhan, Feb 14, 2015
*/

module SimpleMPI {

  use SysCTypes;

  // External types
  extern type MPI_Comm; // Opaque type for MPI communicator 

  // Define MPI_COMM_WORLD
  extern const MPI_COMM_WORLD : MPI_Comm;

  // The MPI functions we need
  extern proc MPI_Init(argc, argv) : c_int;
  extern proc MPI_Initialized(ref flag : c_int) : c_int;
  extern proc MPI_Finalize() : c_int;
  extern proc MPI_Barrier(comm : MPI_Comm) : c_int;
  extern proc MPI_Comm_rank(comm : MPI_Comm, ref rank : c_int) : c_int;
  extern proc MPI_Comm_size(comm : MPI_Comm, ref size : c_int) : c_int;

  // Did we initialize MPI?
  var isInit : bool = false; 


  // Convenience wrappers
  proc finalize() {
    if isInit {
      coforall loc in Locales {
        on loc do MPI_Finalize();
      }
    }
  }

  proc rank() : int {
    var _rank : c_int;
    MPI_Comm_rank(MPI_COMM_WORLD,_rank);
    return _rank;
  }
  proc size() : int {
    var _size : c_int;
    MPI_Comm_size(MPI_COMM_WORLD,_size);
    return _size;
  }
  proc barrier() {
    coforall loc in Locales {
      on loc do MPI_Barrier(MPI_COMM_WORLD);
    }
  }


  // Module initialization will set this up
  var _flag : c_int;
  MPI_Initialized(_flag);
  if (_flag == 0) {
    isInit = true;
    coforall loc in Locales {
      on loc do MPI_Init(0, 0); // Send NULLS in.
    }
  }


}
