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
const multiOnlyPathSep = '/////';
const pathSep2 = '//';


proc collapse(name: string) {
   writeln(name, ' becomes ', normPath(name));
}


// No change when given a valid path.
collapse(collapsedA);
// Collapse redundant separators.
collapse(collapseToA0);
// Eliminate trailing path separators.
collapse(collapseToA1);
// Eliminate current directory symbols.
collapse(collapseToA2);
// Properly collapse up-level references.
collapse(collapseToA3);
// Empty string yields single current dir.
collapse('');
// Single current dir yields single current dir.
collapse(curDir);
// Single parent dir yields single parent dir.
collapse(parentDir);
// Only current dirs yields single current dir.
collapse(multiCurDir);
// No change when given only parent dirs.
collapse(multiParentDir);
// Mix of current/parent dirs yields only parent dirs.
collapse(multiMixedParentCurDir);
// More than two path separators yields single path separator.
collapse(multiOnlyPathSep);
// No change when given single path separator.
collapse(pathSep);
// No change when given two path separators.
collapse(pathSep2);
// Path separators followed by parent dirs yields one path separator.
collapse(multiOnlyPathSep + multiParentDir);
// Path separators followed by current dirs yields one path separator.
collapse(multiOnlyPathSep + multiCurDir);
