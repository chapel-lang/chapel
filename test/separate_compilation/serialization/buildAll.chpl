// This test exists in support of the 'testGenLib.chpl' test.
// For 'testGenLib.chpl', we want to try building when all relevant files
// are .dyno files. We can't prevent 'testGenLib.chpl' from being passed to
// 'chpl' by start_test, and so this helper test exists to list all the
// desired .dyno files on the command line. 'testGenLib' will serve as the
// "main" module for this compilation.
;
