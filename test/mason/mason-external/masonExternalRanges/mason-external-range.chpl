use MasonModify;
use MasonExternal;
use MasonBuild;
use MasonInit;

proc main() {
  // initialise project
  const initArgs = ['mason', 'init'];
  masonInit(initArgs);
  // masonExternalAdd
  const args: [0..3] string;
  args = ['mason','add','--external','libtomlc99@0.2019.03.06:0.2019.06.24'];
  masonModify(args);
  //setup spack
  const setupArgs = ['mason','external','--setup'];
  masonExternal(setupArgs);
  //install package
  var installArgs: [0..3] string = ['mason', 'external', 'install', 'libtomlc99@0.2019.06.24'];
  installSpkg(installArgs);
  // //build
  var buildArgs: [0..2] string = ['mason', 'build', '--force'];
  masonBuild(buildArgs);
}
