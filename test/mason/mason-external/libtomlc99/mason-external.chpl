/*
 To test with debug information, run these commands in this directory:

    source EXECENV
    mason external --setup
    mason external install libtomlc99@0.2019.06.24
    mason build --force --show

*/
use MasonBuild;
use MasonExternal;

proc main() {

  // install Spack
  setupSpack();

  // Download and install openBLAS 0.2.20 and build with GCC
  var args: [0..3] string = ["mason", "external", "install", "libtomlc99@0.2019.06.24"];
  installSpkg(args);

  // build library
  var buildArgs: [0..2] string = ["mason", "build", "--force"];
  masonBuild(buildArgs);

}



