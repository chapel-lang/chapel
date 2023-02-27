use IO, IO.FormattedIO, Regex;

// strings
urls(true, openreader("urls.txt"), compile("https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?"), false);
urls(false, openreader("urls.txt"), compile("https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?"), false);
urls(true, openreader("urls.txt"), compile("https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?"), true);
urls(false, openreader("urls.txt"), compile("https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?"), true);

// bytes
urls(true, openreader("urls.txt"), compile(b"https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?"), false);
urls(false, openreader("urls.txt"), compile(b"https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?"), false);
urls(true, openreader("urls.txt"), compile(b"https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?"), true);
urls(false, openreader("urls.txt"), compile(b"https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?"), true);

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
