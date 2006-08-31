iterator jam( max: int): int {
  var i, j: int;
  i = 0;
  while (i <= max) {
    if (i % 2) {
      yield i;
    } else {
      yield i*2;
    }
    i = i + 1;
  }

  j = 2;
  while (j <= max/2) {
    yield j;
    j = j + 1;
  }
}


def test1() {
  for k in jam( 10) {
    write( " ", k);
  }
  writeln();
}

// Trying to duplicate the above.
class jelly {
  var  max: int;
  var  i, j: int;

  def getHeadCursor() {
    return getNextCursor( 0);
  }

  def getNextCursor( jump_to: int) {
    select jump_to {
      when 0 goto return_0;
      when 1 goto return_1;
      when 2 goto return_2;
      when 3 goto return_3;
      otherwise halt( "unknown jump_to state");
    }

    label return_0;
    i = 0;
    while (i <= max) {
      if (i % 2) {
        return 1;
        // yield i;
        label return_1;
      } else {
        return 2;
        // yield i*2;
        label return_2;
      }
      i = i + 1;
    }

    j = 2;
    while (j <= max/2) {
      return 3;
      // yield j;
      label return_3;
      j = j + 1;
    }

    return 4;
  }

  def getValue( jump_to: int) {
    select jump_to {
      when 1 return i;
      when 2 return i*2;
      when 3 return j;
      otherwise halt( "unknown return value");
    }
  }

  def isValidCursor?( jump_to) {
    return jump_to != 4;
  }
}


def jam_new( max: int) {
  var jelly_i: jelly = jelly();
  jelly_i.max = max;
  return jelly_i;
}


def test2() {
  for m in jam_new( 10) {
    write( " ", m);
  }
  writeln();
}

      
test1();
test2();
