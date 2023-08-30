# README

This provides a simple syntax highlighter for Chapel AST in the TextMate bundle format, which is commonly used by VSCode to do syntax highlighting.

## Install in VSCode

Install locally in VSCode with `cd $CHPL_HOME/highlight/ast/tmbundle && ln -s $(pwd) $HOME/.vscode/extensions/chapel-ast-vscode-syntax-highlighter`.

> Note: you must install it as 'chapel-ast-vscode-syntax-highlighter', do not change this. This has to match the 'name' defined in package.json

## Grammar structure

The primary file which implements the grammar is `./syntaxes/chapel-ast.tmLanguage.json`. The three main sections are `keywords`, `astid`, and `flags`.

- `keywords` contains special words that get called out in the AST.

  - There are two sections for `keyword.control.ast`, one handles keywords which are bare and the other handles keywords which are wrapped in single quotes. Both can be easily extended if there are other interesting keywords.
  - There are also two sections for `storage.type.ast`, specifically to handle the special case of `const?`.
  - There is a special case for `this` so it gets highlighted differently.

- `astid` highlights numbers in the form of `[###]` and `(###` (for statement ids).

- `flags` is really just a highlighter for strings, with some special logic for when strings are truncated to prevent formatting issues. This happens because the AST dumper for strings only uses the first part of long strings.
