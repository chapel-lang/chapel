record BeerAdvocateRecord {
  var beer_name: string;
  var beer_beerId: int;
  var beer_brewerId: int;
  var beer_ABV: real;
  var beer_style: string;
  var review_appearance: real;
  var review_aroma: real;
  var review_palate: real;
  var review_taste: real;
  var review_overall: real;
  var review_time: int;
  var review_profileName: string;
  var review_text: string;
}

// ==== IndexOf(ss: string, s: string, start: int) -- Given a string s, we return the
// ==== index of the first occurence of the substring ss after start.
proc indexOf(substring: string, s: string, start: int): int {

  if (substring.size > s.size) {
    return -1;
  }

  for i in start..(s.size - substring.size + 1) {
    //writeln("i: " + i + ", s.size:" + s.size + ", substring.size: " + substring.size);
    var ss = s[i..#substring.size];
    //writeln("ss: " + ss);
    if (ss == substring) {
      return i;
    }
  }

  return -1;
}

// ==== deserializeRecord(s: string) -- Parses a review into its internal
// ==== representation as a record
proc deserializeRecord(s: string): BeerAdvocateRecord {
  // FIXME: We don't need this whole record for the current purposes of this test. We
  // seem to be doing extra string work/using more memory and doing more work then we
  // need to necessarily (IMO)
  var res: BeerAdvocateRecord;
  var base: int = 1;
  (res.beer_name, base) = findFieldValue(s, "beer/name: ", base);
  (res.beer_beerId, base) = findFieldValueInt(s, "beer/beerId: ", base);
  (res.beer_brewerId, base) = findFieldValueInt(s, "beer/brewerId: ", base);
  (res.beer_ABV, base) = findFieldValueReal(s, "beer/ABV: ", base);
  (res.beer_style, base) = findFieldValue(s, "beer/style: ", base);
  (res.review_appearance, base) = findFieldValueReal(s, "review/appearance: ", base);
  (res.review_aroma, base) = findFieldValueReal(s, "review/aroma: ", base);
  (res.review_palate, base) = findFieldValueReal(s, "review/palate: ", base);
  (res.review_taste, base) = findFieldValueReal(s, "review/taste: ", base);
  (res.review_overall, base) = findFieldValueReal(s, "review/overall: ", base);
  (res.review_time, base) = findFieldValueInt(s, "review/time: ", base);
  (res.review_profileName, base) = findFieldValue(s, "review/profileName: ", base);
  return res;
}

// ==== findFieldValue(s: string, fieldStart: string, base: int) -- Returns a tuple
// ==== (field, value_of_field). i.e (beer, beerID)
proc findFieldValue(s: string, fieldStart: string, base: int): (string, int) {
  var i = indexOf(fieldStart, s, base);
  if (i == -1) {
    return ("", 1);
  }
  var j = indexOf("\n", s, i + fieldStart.size);
  if (j == -1) {
    return ("", 1);
  } else {
    return (s[i + fieldStart.size..(j - 1)], j + 1);
  }
}
// ==== findFieldValueReal(s: string, fieldStart: string, base: int) -- Returns a
// tuple 
proc findFieldValueReal(s: string, fieldStart: string, base: int): (real, int) {
  var (a, b) = findFieldValue(s, fieldStart, base);
  if (a.size > 0) {
    return (a: real, b);
  } else {
    return (0.0, b);
  }
}
// ==== findFieldValueInt(s: string, fieldStart: string, base: int) -- Same here
proc findFieldValueInt(s: string, fieldStart: string, base: int): (int, int) {
  var (a, b) = findFieldValue(s, fieldStart, base);
  if (a.size > 0) {
    // TZ: ???? How is this legal?
    return (a: int, b);
  } else {
    return (0, b);
  }
}

