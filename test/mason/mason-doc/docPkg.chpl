use FileSystem;

use MasonDoc only;

here.chdir('Pkg');

// Document package
MasonDoc.masonDoc(['mason', 'doc']);

const sourcesDir = 'doc/_sources/modules/src/',
      Pkg = sourcesDir + 'Pkg.rst.txt',
      SubPkg = sourcesDir + 'SubPkg.rst.txt',
      FS = sourcesDir + 'FileSystem.rst.txt';

// Confirm Pkg was documented
if !exists(Pkg) then
  writeln('Error: ', Pkg , ' not generated');

// Confirm SubPkg was documented
if !exists(SubPkg) then
  writeln('Error: ', SubPkg , ' not generated');

// Sanity check to confirm we're not creating docs for standard library pkgs
if exists(FS) then
  writeln('Error: ', FS, ' was generated');

