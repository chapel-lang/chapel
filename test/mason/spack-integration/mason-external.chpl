
use MasonBuild;
use MasonExternal;

proc main() {

  // install Spack
  setupSpack();

  // Download and install openBLAS 0.2.20 and build with GCC
  var args: [0..3] string = ["mason", "external", "install", "openblas@0.2.20%gcc"];
  installSpkg(args);

  // build library
  var buildArgs: [0..2] string = ["mason", "build", "--force"];
  masonBuild(buildArgs);

}
