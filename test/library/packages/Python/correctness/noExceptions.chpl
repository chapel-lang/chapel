use Python;

var hello = """
print('Hello, World!')
""";
var hello_raise = """
raise Exception('Hello, World!')
""";

// use default checking
{
  var interp = new Interpreter();
  var mod = interp.importModule('hello', hello);
}

// explicitly use no checking, blocked by https://github.com/chapel-lang/chapel/issues/26579
{
  var interp = new Interpreter(checkExceptions=false);
  var mod = interp.importModule('hello', hello);
}

// explicitly use checking, blocked by https://github.com/chapel-lang/chapel/issues/26579
{
  var interp = new Interpreter(checkExceptions=true);
  try {
    var mod = interp.importModule('hello', hello_raise);
  } catch e: PythonException {
    writeln("Caught exception: ", e.message());
  } catch {
    writeln("Caught unknown exception");
  }
}
