module DefinesMethods {
    proc int.doSomething() {
        writeln("int");
    }
}

module ImportsMethods {
    import DefinesMethods.int;

    proc main(){
        42.doSomething();
    }
}
