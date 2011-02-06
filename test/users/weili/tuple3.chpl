proc fun1(type args ... ?n){
        var val:((...args), real);
        return val;
}

proc fun2(type args ... ?n){
        var val:(((...args)), real);
        return val;
}

writeln(typeToString(fun1(int, bool).type));
writeln(typeToString(fun2(int, bool).type));
