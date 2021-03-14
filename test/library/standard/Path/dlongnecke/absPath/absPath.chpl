use FileSystem;
use Path;


// Assortment of test strings.
const wellFormed0 = 'foo';

const pathSep2 = '//';
const pathSep3 = '///';


proc absolutize(name: string) {
  writeln(name, ' becomes ', absPath(name));
}


// Single parent dir yields path up one level from cwd.
absolutize(parentDir);

// Single current dir yields cwd.
absolutize(curDir);

// Well formed relative path without leading slash yields cwd/path.
absolutize(wellFormed0);

// Path with 1 leading slash yields absolute path, 1 leading slash.
absolutize(pathSep + wellFormed0);

// Path with 2 leading slashes yields absolute path, 2 leading slashes.
absolutize(pathSep2 + wellFormed0);

// Path with 3 leading slashes yields absolute path, 1 leading slash.
absolutize(pathSep3 + wellFormed0);

// Should just print the current working directory (does nothing).
absolutize(here.cwd());
