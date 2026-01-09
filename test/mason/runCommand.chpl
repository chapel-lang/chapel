import MasonUtils;
use IO;
use List;
use UnitTest;

proc CMD do return ["./runCommandHelper", "-nl1"];

proc commandOutputNoError(test: borrowed Test, const cmds...) throws {
  var result: string;
  var cmd: list(string);
  for c in cmds do
    cmd.pushBack(c);
  try {
    writeln("Running command: ", cmd);
    result = MasonUtils.runCommand(cmd.toArray());
  } catch e {
    writeln("Caught error running command: ", e);
    test.assertTrue(false);
  }
  return result;
}

proc simpleOutput(test: borrowed Test) throws {
  var result = commandOutputNoError(test, CMD);
  test.assertNotEqual(result.find("1.0"), -1);
  test.assertEqual(result:real, 1.0);
}

proc manyLines(test: borrowed Test) throws {
  const numLines = 30_000;
  var result = commandOutputNoError(test, CMD, ["--numLines=%?".format(numLines)]);
  var lines = result.split("\n");
  test.assertEqual(lines.size, numLines+1);
}

proc superLongLine(test: borrowed Test) throws {
  const numPerLine = 30_000;
  var result = commandOutputNoError(test, CMD, ["--randomValues=true", "--numPerLine=%?".format(numPerLine)]);
  test.assertEqual(result.find("\n"), result.size - 1);
  // test.assertEqual(result.count(" "), numPerLine - 1);
}

UnitTest.main();
