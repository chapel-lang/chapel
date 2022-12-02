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

0. Fork and clone the [chapel-lang/chapel-tmbundle][1] repo.
0. Manually update the PList/XML in `Syntaxes/Chapel.tmLanguage` with whatever
   changes you need.
0. Test the changes, e.g. using [Jetbrains][4].  Opening a copy of [lulesh][5]
   may be useful.
0. Open a PR for the chapel-tmbundle repo and follow the normal merge process.
0. That's it! The next time [github/linguist][2] is released, the changes will
   automatically be picked up.

[1]: https://github.com/chapel-lang/chapel-tmbundle
[2]: https://github.com/github/linguist
[4]: https://www.jetbrains.com/help/clion/tutorial-using-textmate-bundles.html#importing-bundles
[5]: https://github.com/chapel-lang/chapel/blob/main/test/release/examples/benchmarks/lulesh/lulesh.chpl

If you can figure out how to use the [Textmate bundle editor][3], please update
these instructions accordingly.

[3]: https://manual.macromates.com/en/bundles
