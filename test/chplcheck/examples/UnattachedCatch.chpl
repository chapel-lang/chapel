/*
  The recommend Chapel style is for catch statements to be attached to the
  curly brace of the preceding block, rather than being on a separate line.
*/

// this uses unattached catch statements
try {
  someThrowingFunction();
}
catch e: SpecificError {
  // handle SpecificError
}
catch {
  // handle all other errors
}

// this uses attached catch statements
try {
  someThrowingFunction();
} catch e: SpecificError {
  // handle SpecificError
} catch {
  // handle all other errors
}

