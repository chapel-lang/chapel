module DefinesMethods {
    proc string.doSomething() {
        writeln("string");
    }
    proc locale.doSomething() {
        writeln("locale");
    }
}

module ImportsMethods {
    import DefinesMethods.string;
    import DefinesMethods.locale;

    proc main(){
        "hello".doSomething();
        here.doSomething();
    }
}
