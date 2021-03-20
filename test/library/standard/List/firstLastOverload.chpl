use List;

proc f(args: list(string)){
    var lower = args[0].toLower();
    writeln(lower);
}

proc ref_fun(ref arr: list(string)){
    writeln(arr.first().toLower());
    writeln(arr.last().toLower());
}

proc constref_fun(const ref arr: list(string)){
    writeln(arr.first().toLower());
    writeln(arr.last().toLower());
}

proc main(){
    var args: list(string);
    args.append("HELLO");
    args.append("CHAPEL");
    f(args);
    ref_fun(args);
    writeln(args);
    constref_fun(args);
}