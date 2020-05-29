use MasonModify;
use MasonExternal;
use MasonBuild;
use MasonInit;

proc main() {
  // Initialise a library project
  const initArgs = ['mason', 'init'];
  masonInit(initArgs);
  // Adds external library to Mason.toml
  const args: [0..3] string;
  args = ['mason','add','--external','libtomlc99@0.2019.03.06:0.2019.06.24'];
  masonModify(args);
  // Sets up Spack
  const setupArgs = ['mason','external','--setup'];
  masonExternal(setupArgs);
  // Update compilers.yaml for this system
  var compilerFindArgs = ["mason", "external", "compiler", "--find"];
  masonExternal(compilerFindArgs);
  // Download and install libtomlc99
  var installArgs: [0..3] string = ['mason', 'external', 'install', 'libtomlc99@0.2019.06.24'];
  installSpkg(installArgs);
  // Build library that uses libtomlc99
  var buildArgs: [0..2] string = ['mason', 'build', '--force'];
  masonBuild(buildArgs);
}
