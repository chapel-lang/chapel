
use MasonExample;
use MasonExternal;

proc main() {

  // install Spack
  setupSpack();

  // Download and install openBLAS 0.2.20 and build with GCC
  var args: [0..3] string = ["mason", "external", "install", "openblas@0.2.20%gcc"];
  installSpkg(args);

  // build the examples
  masonExample(["--no-run", "--force"]);

  // run each example
  masonExample(["eigen.chpl", "--no-build"]);

}
