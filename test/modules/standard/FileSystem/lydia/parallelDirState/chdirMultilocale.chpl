use FileSystem;
// This test demonstrates the effect of chdir() and the result of cwd() when
// dealing with multiple locales

// It also ensures that Locales[x].cwd and .chdir work.

writeln(Locales[1].cwd(), " on locale 1");
Locales[1].chdir("foo");
writeln(Locales[1].cwd(), " on locale 1");

writeln(here.cwd(), " on locale 0");
writeln(Locales[1].cwd(), " on locale 1");
