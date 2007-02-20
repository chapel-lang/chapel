module a {
  module b { var a: int; }
}

module b {}

def main() {
  use b;
}
