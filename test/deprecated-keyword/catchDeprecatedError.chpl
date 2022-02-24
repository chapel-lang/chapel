deprecated
class MyDepErr : Error {
}

proc throwsDepErr() throws {
  throw new MyDepErr();
}

proc main() {
  try {
    throwsDepErr();
  } catch (e: MyDepErr) {
    writeln(e.type: string);
  } catch {
    writeln("whatever");
  }
}
