proc dateString() {
  use DateTime;
  return datetime.now().strftime("%Y")[1..2];
}

writeln(dateString());
