use FileSystem, IO;

use MasonDoc only;

enum tests {
  base, withAuthor
}

config const testcase = tests.base;
config const keep = false;

here.chdir('Pkg');

// Document package
if testcase == tests.base then
  MasonDoc.masonDoc(['doc']);
else if testcase == tests.withAuthor then
  MasonDoc.masonDoc(['doc', '--', '--author', 'Billy']);

const sourcesDir = 'doc/_sources/modules/src/',
      Pkg = sourcesDir + 'Pkg.rst.txt',
      SubPkg = sourcesDir + 'SubPkg.rst.txt',
      FS = sourcesDir + 'FileSystem.rst.txt';

writeln("Success: docs generated");

// Confirm Pkg was documented
if !exists(Pkg) then
  writeln('Error: ', Pkg , ' not generated');
else
  writeln("Success: ", Pkg, " was generated");

// Confirm SubPkg was documented
if !exists(SubPkg) then
  writeln('Error: ', SubPkg , ' not generated');
else
  writeln("Success: ", SubPkg, " was generated");

// Sanity check to confirm we're not creating docs for standard library pkgs
if exists(FS) then
  writeln('Error: ', FS, ' was generated');
else
  writeln("Success: ", FS, " was not generated");

if testcase == tests.base {
  // confirm author was added to index.html
  const indexHtml = 'doc/index.html';
  if !exists(indexHtml) then
    writeln('Error: ', indexHtml, ' not generated');
  else {
    const indexContent = IO.openReader(indexHtml).readAll(string);
    if indexContent.find('2025, Caroline') != -1 then
      writeln("Success: Author 'Caroline' was added to index.html");
    else
      writeln("Error: Author 'Caroline' was not added to index.html");
  }
}
if testcase == tests.withAuthor {
  // confirm author was added to index.html and overrode Mason.toml author
  const indexHtml = 'doc/index.html';
  if !exists(indexHtml) then
    writeln('Error: ', indexHtml, ' not generated');
  else {
    const indexContent = IO.openReader(indexHtml).readAll(string);
    if indexContent.find('2025, Billy') != -1 then
      writeln("Success: Author 'Billy' was added to index.html");
    else
      writeln("Error: Author 'Billy' was not added to index.html");
  }
}

if !keep && isDir('doc') {
  FileSystem.rmTree('doc');
}
