/*
   :class:`TestError` is a base class.
*/
class TestError: Error {
  var details: string;

  proc init(details: string = "") {
    this.details = details;  
  }
  
  // Message function overridden here
   override proc message() {
    return this.details;
  }
}

/*Assertion Error class. Raised when assert Function Failed*/
class AssertionError: TestError {
  proc init(details: string = "") {
    super.init(details);
  }
}

/* TestSkipped Error Class. Raised when a test is skipped.*/
class TestSkipped: TestError {
  proc init(details: string = "") {
    super.init(details);
  }
}

/* DependencyFound Error Class. Raised when a all dependency
   of a test are not met.
*/
class DependencyFound: TestError {
  proc init(details: string = "") {
    super.init(details);
  }
}

/* TestIncorrectNumLocales Error Class. Raised when test is not run with
   expected number of Locales.
*/
class TestIncorrectNumLocales: TestError {
  proc init(details: string = "") {
    super.init(details);
  }
}

/* UnexpectedLocales Error Class. Raised when test has
    locales with which it can't be run.
    Eg: MaxLocales < MinLocales
*/
class UnexpectedLocales: TestError {
  proc init(details: string = "") {
    super.init(details);
  }
}
