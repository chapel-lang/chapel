use SpecParser;

/*
  Test Suite for spec parser

  toml listing will be any of the following

  [external]
  hdf5 = "hdf5@1.2.3%clang
  hdf5 = "1.2.3"
  hfd5 = "hdf5@1.2.3"
  hdf5 = "hdf5@1.2.3%clang +hl~mpi"
*/

var spec1 = "openblas@1.2.3%gcc@4.2.3 +hl~mpi";
var spec2 = "hdf5@1.6.3%gcc~mpi";
var spec3 = "hdf5@1.1.3:1.52.3zh"; // compiler will be inferred
var spec4 = "hdf5@1.2.3"; // compiler will be inferred
var spec5 = "hdf5@1.7.3+mpi"; // compiler will be inferred 
var spec6 = "hdf5@1.20.3%f77";
var spec7 = "hdf5@1.11.3%f77+mpi";

// concretized specs from spack.
// repro with command `spack spec ncurses` and `spack spec hdf5`
var spec8 = "ncurses@6.0.0%clang@9.0.0-apple patches=f84b2708a42777aadcc7f502a261afe10ca5646a51c1ef8b5e60d2070d926b57 ~symlinks arch=darwin-sierra-x86_64 ^pkg-config@0.29.2%clang@9.0.0-apple+internal_glib arch=darwin-sierra-x86_64";
var spec9 = "hdf5@1.10.1%clang@9.0.0-apple~cxx~debug~fortran~hl~mpi+pic+shared~szip~threadsafe arch=darwin-sierra-x86_64 ^hwloc@1.11.8%clang@9.0.0-apple~cuda+libxml2~pci arch=darwin-sierra-x86_64 ^libxml2@2.9.4%clang@9.0.0-apple~python arch=darwin-sierra-x86_64 ^xz@5.2.3%clang@9.0.0-apple arch=darwin-sierra-x86_64 ^zlib@1.2.11%clang@9.0.0-apple+optimize+pic+shared arch=darwin-sierra-x86_64";


var specs = [spec1, spec2, spec3, spec4, spec5, spec6, spec7, spec8, spec9];

proc main() {
  for spec in specs {
    writeln("---------");
    writeln("Spec: " + spec);
    const specFields = getSpecFields(spec);
    writeln("Package: " + specFields[0]);
    writeln("Version: " + specFields[1]);
    writeln("Compiler: " + specFields[2]);
    writeln("Variants: " + specFields[3]);
    writeln("----------\n");
  }
}

