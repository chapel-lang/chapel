@unstable "The variable 'msg' is unstable and its interface is subject to change in the future"
config const msg: string = "This is a test";

proc main() {
    //would trigger unstable warning
    //@unstable "This can only be called safely on a string whose home is the current locale. This property can be enforced by calling string.localize() before c_str(). If the string is remote, the program will halt."
    writeln(msg); //localize: A shallow copy if the string is already on the current locale, otherwise a deep copy is performed.
    //localizing before printing current string
}
