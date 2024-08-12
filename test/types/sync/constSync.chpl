// this this is just to make sure that `const` `sync` variables are actually `const`
const foo: sync int = 1;

foo.writeEF(2);
foo.writeFF(2);
foo.writeXF(2);
foo.reset();
