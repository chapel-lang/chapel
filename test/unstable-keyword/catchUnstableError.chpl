@unstable
class MyUnstableErr : Error {
}

proc throwsUnstableErr() throws {
  throw new MyUnstableErr();
}

proc main() {
  try {
    throwsUnstableErr();
  } catch (e: MyUnstableErr) {
    writeln(e.type: string);
  } catch {
    writeln("whatever");
  }
}
