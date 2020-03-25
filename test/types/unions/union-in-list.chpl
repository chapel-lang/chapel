use List;

union Union {
    var s: string;
}

record Container {
    var items: list(Union);
}

proc main() {
    var aUnion: Union;
      aUnion.s = "string literal";

        var container = new Container();
          container.items.append(aUnion);

            writeln(container);
}
