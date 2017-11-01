pragma "error mode fatal"
module mymodule {
  use ExampleErrors;

  proc propError() {
    throwAnError();
  }

  propError();
}
