use FileSystem;
// This test demonstrates the effect of chdir() and the result of cwd() when
// dealing with multiple locales

on Locales[1] {
  writeln(cwd(), " on locale 1");
  here.chdir("foo");
  writeln(cwd(), " on locale 1");
}
writeln(cwd(), " on locale 0"); 
on Locales[1] {
  writeln(cwd(), " on locale 1");
}
