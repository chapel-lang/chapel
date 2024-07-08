@unstable("The variable 'msg' is unstable and its interface is subject to change in the future")
config const msg: string = "This is a test";

proc main() {
    writeln(msg); //triggers unstable warning
}
