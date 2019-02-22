/*
   proc normPath(name: string): string;

   Collapse paths such as `foo//bar`, `foo/bar/`, `foo/./bar`, and
   `foo/baz/../bar` into `foo/bar`.  Warning: may alter meaning of paths
   containing symbolic links.  Similar to :proc:`normCase`, on Windows will
   replace forward slashes.

   TODO: Add appropriate tests for Windows machines.
*/


use Path;


// As per the examples given in the description of the normPath function.
const collapsedA = 'foo/bar';
const collapseToA0 = 'foo//bar';
const collapseToA1 = 'foo/bar/';
const collapseToA2 = 'foo/./bar';
const collapseToA3 = 'foo/baz/../bar';

// Variables for expanded coverage.
const multiCurDir = '././.';
const multiParentDir = '../../..';
const multiMixedParentCurDir = './.././.././..';
const multiOnlyPathSep = '////';


// Collapse redundant separators.
writeln(normPath(collapseToA0) == collapsedA);

// Eliminate trailing path separators.
writeln(normPath(collapseToA1) == collapsedA);

// Eliminate current directory "." symbols.
writeln(normPath(collapseToA2) == collapsedA);

// Collapse up-level references.
writeln(normPath(collapseToA3) == collapsedA);

// Empty string yields a single current dir.
writeln(normPath('') == curDir);

// Single current dir yields a single current dir.
writeln(normPath(curDir) == curDir);

// Single parent dir yields a single parent dir.
writeln(normPath(parentDir) == parentDir);

// Only current/parent dirs yields only parent dirs.
writeln(normPath(multiMixedParentCurDir) == multiParentDir);

// Only current dirs yields single current dir.
writeln(normPath(multiCurDir) == curDir);

// Only parent dirs yields no change.
writeln(normPath(multiParentDir) == multiParentDir);

// Only path separators yields single path separator.
writeln(normPath(multiOnlyPathSep) == pathSep);

// Path separators followed by parent dirs yields single path separator.
writeln(normPath(multiOnlyPathSep + multiParentDir) == pathSep);

// Path separators followed by current dirs yields single path separator.
writeln(normPath(multiOnlyPathSep + multiCurDir) == pathSep);
