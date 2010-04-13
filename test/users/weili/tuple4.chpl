def fun1(type args ... ?n){
        var val:((...args), real);
        return val;
}

def fun2(type args ... ?n){
        var val:(1*(...args), real);
        return val;
}

writeln(typeToString(fun1(int, bool).type));
writeln(typeToString(fun2(int, bool).type));
