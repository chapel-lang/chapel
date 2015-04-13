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
0. Test the changes using [lightshow][4]. Try this [lulesh example][5].
0. That's it! The next time [github/linguist][2] is released, the changes will
   automatically be picked up.

[1]: https://github.com/chapel-lang/chapel-tmbundle
[2]: https://github.com/github/linguist
[4]: https://github-lightshow.herokuapp.com/
[5]: https://github-lightshow.herokuapp.com/?utf8=%E2%9C%93&scope=from-url&grammar_url=https%3A%2F%2Fgithub.com%2Fthomasvandoren%2Fchapel-tmbundle%2Fblob%2Fmaster%2FSyntaxes%2FChapel.tmLanguage&grammar_text=&code_source=from-url&code_url=https%3A%2F%2Fgithub.com%2Fchapel-lang%2Fchapel%2Fblob%2Fmaster%2Ftest%2Frelease%2Fexamples%2Fbenchmarks%2Flulesh%2Flulesh.chpl&code=

If you can figure out how to use the [Textmate bundle editor][3], please update
these instructions accordingly.

[3]: https://manual.macromates.com/en/bundles
