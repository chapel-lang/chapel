extern proc printf(fmt, args...);

// The string literals should be preserved as c_strings when being passed into
// the generic args of printf because it is extern. If printf wasn't extern
// they would be converted to string.
printf("%s\n", "Hello, World!");
