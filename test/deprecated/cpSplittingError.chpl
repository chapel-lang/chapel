proc splitString(s: string, needle: string) {
  try {
    return s[s.find(needle)];
  } catch e:CodepointSplittingError {
    return "";
  }
}
