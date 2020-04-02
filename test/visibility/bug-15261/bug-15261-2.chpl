module MHDF5 {
  import SysCTypes;

  public import this.C_HDF5;
  module C_HDF5 {
    import SysCTypes;

    public import this.HDF5_WAR;
    module HDF5_WAR {
      proc H5LTmake_dataset_WAR() {
        writeln("in H5LTmake_dataset_WAR");
      }
    }
  }
}

module GenSymIO {
  import MHDF5;
  import MHDF5.C_HDF5;

  proc write1DDistArray() throws {
    coforall i in 1..1 {
      import C_HDF5.HDF5_WAR;
      HDF5_WAR.H5LTmake_dataset_WAR();
    }
  }

  proc main() {
    try! {
      write1DDistArray();
    }
  }
}
