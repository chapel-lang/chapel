module Main {
  @unstable("This file is unstable")
  module Unstable {
      record snake_case_rec {}
      var snake_case_var = 42;
      proc snake_case_proc() {}

      record myprefix_snake_case_rec {}
      var myprefix_snake_case_var = 42;
      proc myprefix_snake_case_proc() {}
  }

  module Stable {
      record snake_case_rec {}
      var snake_case_var = 42;
      proc snake_case_proc() {}

      record myprefix_snake_case_rec {}
      var myprefix_snake_case_var = 42;
      proc myprefix_snake_case_proc() {}
  }
}
