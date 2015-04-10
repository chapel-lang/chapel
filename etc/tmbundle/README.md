Chapel Textmate bundle
======================

The Chapel Textmate bundle is primarily used for syntax highlighting Chapel
code on Github. It would be great if a Textmate user was willing to improve and
expand it, but for now the primary goal is to keep it up-to-date with keywords
and grammar changes.

The Textmate bundle was originally created by [bholt][0].

[0]: https://github.com/bholt

Updating Chapel bundle
----------------------

1. Fork and clone the [chapel-lang/chapel-tmbundle][1] repo.
2. Manually update the PList/XML in `Syntaxes/Chapel.tmLanguage` with whatever
   changes you need.
3. That's it! The next time [github/linguist][2] is released, the changes will
   automatically be picked up.

[1]: https://github.com/chapel-lang/chapel-tmbundle
[2]: https://github.com/github/linguist

If you can figure out how to use the [Textmate bundle editor][3], please update
these instructions accordingly.

[3]: https://manual.macromates.com/en/bundles
