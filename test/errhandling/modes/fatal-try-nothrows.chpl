pragma "error mode fatal"
module mymodule {
  use ThrowError;

  proc propError() {
    throwAnError();
  }

  propError();
}
