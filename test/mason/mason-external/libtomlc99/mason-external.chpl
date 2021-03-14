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

  // Update compilers.yaml for this system
  var compilerFindArgs = ["mason", "external", "compiler", "--find"];
  masonExternal(compilerFindArgs);

  // Download and install libtomlc99
  var args = ["mason", "external", "install", "libtomlc99@0.2019.06.24"];
  masonExternal(args);

  // build library that uses libtomlc99
  var buildArgs = ["mason", "build", "--force"];
  masonBuild(buildArgs);

}



