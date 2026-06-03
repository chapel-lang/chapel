use MasonUtils;
import ThirdParty.Pathlib.path;


proc main() {

  // test that cloneSource falls back to https if git clone fails with an ssh url, and that it throws an error if both attempts fail
  try {
    cloneSource("git@github.com:unknown/repo.git", "dummy":path);
  } catch e {
    writeln(e.message());
  }
}
