// This test replaces an automatically-included standard library file with
// a user one because ChapelIO.chpl exists as a sibling to this file.

// Anna 2023-05-22: Replacing the use of AutoMath with ChapelIO, as changes in
// module init order have broken this test for AutoMath. It will break again if
// the "real" ChapelIO moves early enough in module init order that we encounter
// it before our substitute.
// Original comment below:

// While this test uses AutoMath, as of this writing, there are 4 automatic
// standard libraries: AutoMath, Errors, ChapelIO, and Types.
// A workaround in parser.cpp allows this replacing process for these
// except for Errors (because it runs into another workaround in the parser).
// See issue #19313 for a non-workaround proposal to address this.

// One challenge here is that there can only be one copy of a top-level
// module in a given compilation. That means that if this test uses
// ./AutoMath.chpl as the AutoMath module, the internal library will
// refer to it for things like min/max instead of the real one.

// A related challenge to that, and part of the issue with Errors, is that in
// order for this to work, the first use of AutoMath that the compiler seeks
// to resolve has to be this one (vs one coming from somewhere else, e.g. an
// already-parsed included module). That presents an ordering constraint that
// causes problems when doing incremental or separate compilation. This issue,
// combined with the limited number of modules to which this can apply, make
// it potentially not worthwhile to support this pattern.

use ChapelIO;

testchapelio();
writeln("In my foo2");
