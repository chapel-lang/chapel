private use List;
use MasonTest;

proc main(args: [] string) {
    var compopts: list(string) = ["mason", "--recursive"];
    for arg in args[1..] do compopts.append(arg);
    masonTest(compopts);
}
