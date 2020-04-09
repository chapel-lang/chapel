var shared = 0;

proc f(shared) {
}

class shared {
  var shared =0;
}

type shared = real;

proc shared() {
}

proc test() {
  try {
  } catch shared {
  }
}
