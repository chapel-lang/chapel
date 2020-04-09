proc main() {
  var fish: [1..1] owned Fish? = [new owned Salmon?()];
  fish[1]!.spawn();
  writeln(fish);
}

class Fish {
  proc spawn(): owned Fish { return new owned Fish(); }
}

class Salmon: Fish {
  override proc spawn(): owned Fish {
    var offspring = new owned Salmon();
    return offspring;
  }
}
