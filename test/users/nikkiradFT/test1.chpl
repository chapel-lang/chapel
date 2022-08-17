// @unstable "This module is unstable and its interface is subject to change in the future."
// use GPUDiagnostics;

proc main() {
    unstable "The variable 'msg' is unstable"
    config const msg: string = "This is test 1";
    //would probably trigger unstable warning
    //@unstable "This can only be called safely on a string whose home is the current locale. This property can be enforced by calling string.localize() before c_str(). If the string is remote, the program will halt."
    writeln(msg); //localize: A shallow copy if the string is already on the current locale, otherwise a deep copy is performed.
    //localizing before printing current string
}

/*
compiler would recognize when it's been updated
so if the user has to write their own unstable tag and apply it to their uses of it, it wouldn't help them
using it where I'd assume it'd be called

put GPUDiagnostic separately -- would be triggered when called

add .good file
no localizing necessary
*/