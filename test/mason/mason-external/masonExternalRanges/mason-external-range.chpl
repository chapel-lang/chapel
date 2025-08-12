use MasonModify;
use MasonExternal;
use MasonBuild;
use MasonInit;

proc main() {
  // Sets up Spack
  setupSpack();
  // Update compilers.yaml for this system
  var compilerFindArgs = ["external", "compiler", "--find", "--quiet"];
  masonExternal(compilerFindArgs);
  // Download and install libtomlc99
  var installArgs = ["install", "libtomlc99@0.2019.06.24", "--quiet"];
  installSpkg(installArgs);
  // Build library that uses libtomlc99
  var buildArgs = ["build", "--force"];
  masonBuild(buildArgs);
}
