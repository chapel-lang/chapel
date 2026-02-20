use FileSystem, IO;

use MasonLint only;
use MasonUtils only;

here.chdir('Pkg');

try! {
  MasonLint.masonLint(["lint"]);
  writeln("Error: linting passed unexpectedly");
} catch e: MasonUtils.MasonError {
  writeln("Got Lints as expected - ", e.message());
}

try! {
  // TODO: passing cli args overrides whats in the config file
  // https://github.com/chapel-lang/chapel/issues/28330
  MasonLint.masonLint(["lint", "--", "--disable-rule=CamelCaseFunctions"]);
  writeln("Error: linting passed unexpectedly");
} catch e: MasonUtils.MasonError {
  writeln("Got Lints as expected - ", e.message());
}
