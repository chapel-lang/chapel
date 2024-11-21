use Python;
use List;


const html =
' \
<html> \
  <head> \
    <title>Sample HTML</title> \
  </head> \
  <body> \
    <h1>Sample Page</h1> \
    <p>This is a <a href="http://example.com">link</a> to example.com.</p> \
    <p>Here is another <a href="http://chapel-lang.org">link</a> to the Chapel website.</p> \
    <p>And one more <a href="http://github.com">link</a> to GitHub.</p> \
  </body> \
</html> \
';

proc main() {

  var interp = new Interpreter();
  var mod = new Module(interp, "bs4");

  var cls = new Class(mod, "BeautifulSoup");
  var soup = cls(html, 'html.parser');

  var res: list(owned ClassObject?);
  res = soup.call(res.type, "find_all", "a");
  for c in res {
    var linkText = c!.getAttr(string, "text");
    var linkUrl = c!.call(string, "__getitem__", "href");
    writeln(linkText, ": ", linkUrl);
  }
}

