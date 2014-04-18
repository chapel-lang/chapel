module externMethod {

  proc blah3(num) {
    var res = num % 17;
    res = res + 4;
    res = res * 3;
    res = res - 7;
    return res / 8;
  }
}