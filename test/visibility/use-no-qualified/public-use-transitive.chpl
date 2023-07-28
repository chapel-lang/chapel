module Source {
    record R{}
}

module Pipe1 {
    public use Source;
}

module Pipe2 {
    public use Pipe1;
}

module Destination {
    import Pipe2.R;

    proc main() {
        var x: R;
        writeln(x);
    }
}
