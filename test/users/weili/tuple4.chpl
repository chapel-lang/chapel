proc fun1(type args ... ?n){
        var val:((...args), real);
        return val;
}

proc fun2(type args ... ?n){
        var val:(1*(...args), real);
        return val;
}

writeln(fun1(int, bool).type:string);
writeln(fun2(int, bool).type:string);
