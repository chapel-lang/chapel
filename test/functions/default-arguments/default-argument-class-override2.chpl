/*
Chapel code example to illustrate issue with inheritance, overriding methods and named vars.
Declare a base class Foo with a hello proc.  It has 3 named args with defaults.
Next Declare Bar which inherits from Foo and overrides Foo's hello proc with its own
default values for the named args.
When we declare a Foo with a concrete implementation of Bar we see that the Bar hello proc
gets invoked as expected, but the default args do not; they are still Foo's defaults.
Since the overridden proc of Bar's gets invoked, I would have expected Bar's defaults to go
along with it, but that's not the case based on the output.
*/

class Foo {

    proc hello(greeting:string="foo default greeting", prefix:string="FOO_default_prefix", suffix:string="FOO_default_suffix"): string throws {
        return "FOO.hello(...)=> greeting:`" + greeting + "`, prefix:`" + prefix + "`,  suffix:`" + suffix + "`";
    }

}

class Bar : Foo {

    override proc hello(greeting:string="bar default greeting", prefix:string = "default_bar[", suffix:string = "]bar_default"): string throws {
        return "I'm in BAR.hello(...)=> greeting:`" + greeting + "`, prefix:`" + prefix + "`,  suffix:`" + suffix + "`";
    }
}




// Create Foo with concrete Foo impl
// 1. Prove we get the defaults
// 2. Pass in greeting and get defaults on prefix & suffix
// 3. Pass in greeting & prefix, get default on suffix
var f:Foo = new Foo();
writeln("----Foo----");
writeln(f.hello());
writeln(f.hello(greeting="hello"));
writeln(f.hello(greeting="hello", prefix="<<<"));

// Create Bar with concrete Bar impl
// 1. Prove we get Bar's defaults
// 2. Pass in greeting and get Bar's defaults on prefix & suffix
// 3. Pass in greeting & prefix, get Bar's default on suffix
var b:Bar = new Bar();
writeln("----Bar----");
writeln(b.hello());
writeln(b.hello(greeting="I'm Bar"));
writeln(b.hello(greeting="I'm Bar", prefix="<<<"));

// Create Foo with conreate Bar impl.
// This uses Bar's overridden proc hello, but doesn't get the Bar default named args, why?
// 1. We see it inokes Bar's hello method in the output, but we get Foo's named defaults
// 2. Pass in greeting, we see Bar's method invoked, but still get Foo defaults on prefix & suffix
// 3. Pass in greetin & suffix, same thing, We see Bar's output message, but still Foo default suffix.
var fb:Foo = new Bar();
writeln("----Foo with Bar as impl----");
writeln(fb.hello());
writeln(fb.hello(greeting="expect Bar defaults"));
writeln(fb.hello(greeting="expect Bar defaults minus passed in prefix", prefix="<<<"));
