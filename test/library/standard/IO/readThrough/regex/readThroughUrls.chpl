use IO, Regex;

// strings
const url = compile("https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?");
urls(true, openreader("urls.txt"), url, false);
urls(false, openreader("urls.txt"), url, false);
urls(true, openreader("urls.txt"), url, true);
urls(false, openreader("urls.txt"), url, true);

// bytes
const url_b = compile(b"https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?");
urls(true, openreader("urls.txt"), url_b, false);
urls(false, openreader("urls.txt"), url_b, false);
urls(true, openreader("urls.txt"), url_b, true);
urls(false, openreader("urls.txt"), url_b, true);

proc urls(param reuseBuffer, r, reg: regex(?t), stripSep) where reuseBuffer==true {
  var s: t;
  while r.readThrough(s, reg, -1, stripSep) {
    write(s);
  }
}

proc urls(param reuseBuffer, r, reg: regex(?t), stripSep) where reuseBuffer==false {
  while true {
    try {
      write(r.readThrough(reg, -1, stripSep));
    } catch {
      break;
    }
  }
}
