module NewTypeMod {
    record NewType {
        var x: int;

        proc writeThis(f) throws {
            f <~> "NewType with " <~> x;
        }
    }
}

module Mod {
    private use NewTypeMod;

    proc makeNewType() {
        return new NewType(42);
    }
    proc wr(x: NewType) {
        writeln(x);
    }
}

module User {
    private use Mod;

    proc main() {
        var x = makeNewType();
        writeln(x);
        wr(x);
    }
}
