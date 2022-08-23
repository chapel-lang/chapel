@unstable param a: bool = true;
param b: bool = false;

proc main() {
    writeln(a);  //triggers warning
    writeln(b);  //no warning
}