module foo {
    import IO;
    class bar {
        const stream: IO.fileWriter(?);
        proc init(stream: IO.fileWriter(?) = IO.stdout) {
            this.stream  = stream;
        }
        proc deinit() {
            try! stream.flush();
        }
    }
    var test = new bar();
}
