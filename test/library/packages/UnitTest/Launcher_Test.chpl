use MasonTest;
proc main(args: [] string) {
    var compopts = ["mason","--recursive"]; // This is just a Hack. :)
    for arg in args[1..] do compopts.push_back(arg);
    masonTest(compopts);
}