/*
 * This is sugary, but after years of suffering in C, I love that you
 * can print out enumerations in this language, and set them on the
 * command-line like other config vars.
 */

enum people {Brad, David, MaryBeth, Samuel, Steve};

config const me = people.Brad;

writeln("For this run of the program, I am ", me);
