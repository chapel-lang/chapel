use IO, Regex;

// strings
const url = new regex("https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?");
urls(true, openReader("urls.txt", locking=false), url, false);
urls(false, openReader("urls.txt", locking=false), url, false);
urls(true, openReader("urls.txt", locking=false), url, true);
urls(false, openReader("urls.txt", locking=false), url, true);

// bytes
const url_b = new regex(b"https?:\\/\\/(.*\\.)(org|com)([\\/a-zA-Z]*)?(\\.html)?");
urls(true, openReader("urls.txt", locking=false), url_b, false);
urls(false, openReader("urls.txt", locking=false), url_b, false);
urls(true, openReader("urls.txt", locking=false), url_b, true);
urls(false, openReader("urls.txt", locking=false), url_b, true);

proc urls(param reuseBuffer, r, reg: regex(?t), stripSep) where reuseBuffer==true {
  var s: t;
  while r.readThrough(reg, s, -1, stripSep) {
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
