/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
  A lexical anayzer class for simple shell-like syntaxes.
  Implementation and design inspired by `Python's Shlex module
  <https://docs.python.org/3/library/shlex.html>`_.
  This module provides support for split, join and quote operations.
*/

module Shlex {
  private use List;
  private use Regex;
  class shlex {
    /* Input string from which shlex instance is reading characters. */
    var instream: string;

    /* Parsing rules can be set as POSIX or non-POSIX. */
    var posix: bool;

    @chpldoc.nodoc
    var lineno: int;

    /* Make the program verbose. */
    param debug: int;

    /* String of characters which are considered as comment beginners.*/
    var commenters: string;

    /* String of characters that will accumulate multi-character tokens. */
    var wordchars: string;

    /* String of characters which are considered as whitespaces and are
      skipped. By default, it will include space, tab, linefeed
      and carriage-return.
    */
    var whitespace: string;

    /* If `true`, tokens will only be split in whitespaces. */
    var whitespace_split: bool;

    /* Characters that are considered as string quotes. */
    var quotes: string;

    @chpldoc.nodoc
    var escapse: string;

    @chpldoc.nodoc
    var escapsedquotes: string;

    @chpldoc.nodoc
    var state: string;

    @chpldoc.nodoc
    var token: string;

    @chpldoc.nodoc
    var tokindex: int;

    /* String of characters that will be considered as a punctuation.
      Runs of punctuation characters will be returned as a single token.
    */
    var punctuation_chars: string;

    @chpldoc.nodoc
    var pushback: list(string);

    @chpldoc.nodoc
    var _pushback_chars: list(string);

    /*
      Initializes the required variables of class `shlex`.
      Used in new expressions.

      :arg instream: The string to be parsed by shlex.
      :type instream: `string`
      :arg posix: Argument to decide parsing rules (POSIX or non-POSIX).
                  Defaults to `false`.
      :type posix: `bool`
      :arg punctuation_chars: If set to true, contiguous runs of punctuation
                              characters will be returned as a single token.
                              Defaults to `false`.
      :type punctuation_chars: `bool`
      :arg commenters: Strings of characters that are recognized as comment
                        beginners. All characters from comment beginners to end
                        of line are ignored. Defaults to `#`.
      :type commenters: `string`
      :arg custom_punctuation_chars: All the character present in this string
                                      will be considered as punctuation
                                      characters. NOTE: This field is only used
                                      if the argument `punctuation_chars` is
                                      set to true. Defaults to `();|&`.
      :type custom_punctuation_chars: `string`
    */
    proc init(instream:string, posix: bool = false,
              punctuation_chars: bool = false, commenters: string = '#',
              custom_punctuation_chars: string = '();<>|&') {
      this.instream = instream.strip(trailing = true);
      this.posix = posix;
      this.lineno = 1;
      this.debug = 0;
      this.commenters = commenters;
      this.wordchars = "abcdfeghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
      this.wordchars += "0123456789_";
      if posix {
        this.wordchars += "ßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýþÿÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐ";
        this.wordchars += "ÑÒÓÔÕÖØÙÚÛÜÝÞ";
      }
      this.whitespace = " \t\n\r";
      this.whitespace_split = false;
      this.quotes = "\"'";
      this.escapse = "\\";
      this.escapsedquotes = '"';
      this.state = " ";
      this.token = '';
      this.tokindex = 0;
      if (punctuation_chars == false) {
        this.punctuation_chars = '';
      }
      else {
        if custom_punctuation_chars == '' then
        this.punctuation_chars = '();<>|&';
        else this.punctuation_chars = custom_punctuation_chars;
        this.wordchars += '~-./?*=';
        for c in this.punctuation_chars {
          this.wordchars = this.wordchars.replace(c, '');
        }
      }
    }

    /*
      Returns the next token from `instream`. Returns empty string when EOF
      is reached.

      :returns: Next token from `instream`.
      :rtype: `string`

    */
    proc get_token(): string {
      if !this.pushback.isEmpty() {
        var tok = this.pushback.pop(0);
        if this.debug >= 1 {
          writeln("shlex: popping token " + tok);
        }
        return tok;
      }
      if(this.tokindex == -1) {
        return '';
      }
      var raw = try! this.read_token();
      if this.debug >= 1 {
        if(raw != '') {
          writeln("shlex: token = ", raw);
        }
        else {
          writeln("shlex: token = EOF");
        }
      }
      return raw;
    }

    /*
      Parses through `instream` and returns the next token.

      :returns: Next token from `instream`.
      :rtype: `string`
      :throws Error: Unbalanced quotes or incorrect use of escaped character.
    */
    proc read_token(): string throws {
      var quoted:bool = false;
      var escapsedstate:string = ' ';
      var nextchar:string = '';
      while true {
        if this.tokindex == -1 then break;
        if (this.punctuation_chars != '' && !this._pushback_chars.isEmpty()) {
          nextchar = this._pushback_chars.popBack();
        }
        else if(this.tokindex < this.instream.size) {
          nextchar = this.instream[this.tokindex];
          this.tokindex += 1;
        }
        else {
          nextchar = '';
          this.tokindex = -1;
        }
        if this.debug >= 3 then writeln("shlex: in state ", this.state,
                                        "I see character: ", nextchar);
        if nextchar == '\n' then this.lineno += 1;
        if this.state == '' {
          this.token = '';
          break;
        }
        else if this.state == ' ' {
          if nextchar == '' {
            this.state = '';
            break;
          }
          else if (this.whitespace.find(nextchar) != -1) {
            if this.debug >= 2 {
              writeln("shlex: I see whitespaces in whitespace state");
            }
            if (this.token != '' || (this.posix && quoted)) then break;
            else continue;
          }
          else if (this.commenters.find(nextchar) != -1) {
            while(this.tokindex < this.instream.size &&
                    this.instream[tokindex] != '\n') do
              this.tokindex += 1;
            this.lineno += 1;
          }
          else if (this.posix && this.escapse.find(nextchar) != -1) {
            escapsedstate = 'a';
            this.state = nextchar;
          }
          else if (this.wordchars.find(nextchar) != -1) {
            this.token = nextchar;
            this.state = 'a';
          }
          else if (this.punctuation_chars.find(nextchar) != -1) {
            this.token = nextchar;
            this.state = 'c';
          }
          else if (this.quotes.find(nextchar) != -1) {
            if !this.posix then this.token = nextchar;
            this.state = nextchar;
          }
          else if this.whitespace_split {
            this.token = nextchar;
            this.state = 'a';
          }
          else {
            this.token = nextchar;
            if (this.token != '' || (this.posix && quoted)) then break;
            else continue;
          }
        }
        else if (this.quotes.find(this.state) != -1) {
          quoted = true;
          if nextchar == '' {
            if this.debug >= 2 {
              writeln("shlex: I see EOF in quoted state");
            }
            throw new Error("No closing quotation");
          }
          if (nextchar == this.state) {
            if !this.posix {
              this.token += nextchar;
              this.state = ' ';
              break;
            }
            else this.state = 'a';
          }
          else if (this.posix && this.escapse.find(nextchar) != -1 &&
                    this.escapsedquotes.find(this.state) != -1) {
            escapsedstate = this.state;
            this.state = nextchar;
          }
          else {
            this.token += nextchar;
          }
        }
        else if(this.escapse.find(this.state) != -1) {
          if(nextchar == '') {
            if(this.debug >= 2) {
              writeln("I see EOF in escaped state");
            }
            throw new Error("No escaped character");
          }
          if(this.quotes.find(escapsedstate) != -1 && nextchar != this.state &&
              nextchar != escapsedstate) {
            this.token += this.state;
          }
          this.token += nextchar;
          this.state = escapsedstate;
        }
        else if("ac".find(this.state) != -1) {
          if nextchar == '' {
            this.state = '';
            break;
          }
          else if(this.whitespace.find(nextchar) != -1) {
            if this.debug >= 2 then writeln("I see whitespaces in word state");
            this.state = ' ';
            if (this.token != '' || (this.posix && quoted)) then break;
            else continue;
          }
          else if(this.commenters.find(nextchar) != -1) {
            while(instream[this.tokindex] != '\n') do this.tokindex += 1;
            this.lineno += 1;
            if this.posix {
              this.state = ' ';
              if (this.token != '' || (this.tokindex != -1 ||
                  (this.posix && quoted)))
              then break;
              else continue;
            }
          }
          else if(this.state == 'c') {
            if(this.punctuation_chars.find(nextchar) != -1) {
              this.token += nextchar;
            }
            else {
              if(this.whitespace.find(nextchar) == -1) {
                this._pushback_chars.pushBack(nextchar);
              }
              this.state = ' ';
              break;
            }
          }
          else if(this.posix && this.quotes.find(nextchar) != -1) {
            this.state = nextchar;
          }
          else if(this.posix && this.escapse.find(nextchar) != -1) {
            escapsedstate = 'a';
            this.state = nextchar;
          }
          else if((this.wordchars.find(nextchar) != -1) ||
                  (this.quotes.find(nextchar) != -1) || this.whitespace_split)
          {
            this.token += nextchar;
          }
          else {
            if(this.punctuation_chars != '') {
              this._pushback_chars.pushBack(nextchar);
            }
            else {
              this.pushback.insert(0, nextchar);
            }
            if this.debug >= 2 {
              writeln("shlex: I see punctuation in word state");
            }
            this.state = ' ';
            if(this.token != '' || (this.posix && quoted)) then break;
            else continue;
          }
        }
      }
      var result = this.token;
      this.token = '';
      if(this.debug >= 2) {
        if result != '' {
          writeln("shlex: raw token: "+result);
        }
        else {
          writeln("shlex: raw token = EOF");
        }
      }
      return result;
    }
  }

  /*
    Splits the string according to parsing rules specified and returns the
    list of tokens.

    :arg s: The string to be parsed by shlex.
    :type s: `string`
    :arg comments: Decides whether `'#'` should be considered as a comment
                  starting character. Defaults to `false`.
    :type comments: `bool`
    :arg posix: Argument to decide parsing rules (POSIX or non-POSIX). Defaults
                to `false`. Defaults to `true`.
    :type posix: `bool`
    :arg punctuation_chars: If set to true, contiguous runs of punctuation
                            characters will be returned as a single token.
                            Defaults to `false`.
    :type punctuation_chars: `bool`
    :arg custom_punctuation_chars: All the character present in this string
                                  will be considered as punctuation characters.
                                  NOTE: You can also use this field without
                                  setting `punctuation_chars` to true.
                                  Defaults to `''` (empty string)
    :type custom_punctuation_chars: `string`
  */
  proc split(s:string, comments:bool = false, posix:bool = true,
              punctuation_chars: bool = false,
                custom_punctuation_chars: string = "")
  {
    var commenters = '';
    if comments then commenters = '#';
    var lex = new shlex(s, posix = posix, punctuation_chars =
                        (punctuation_chars || custom_punctuation_chars != ''),
                        commenters = commenters,
                        custom_punctuation_chars = custom_punctuation_chars);
    lex.whitespace_split = !(punctuation_chars ||
                              custom_punctuation_chars != '');
    var lst:list(string);
    var pres:string = ' ';
    while(lex.tokindex != -1 || !lex.pushback.isEmpty()) {
      pres = lex.get_token();
      if(posix || pres != '') {
        lst.pushBack(pres);
      }
    }
    return lst;
  }

  @chpldoc.nodoc
  var _find_unsafe = try! new regex("[^\\w@%+=:,./-]");

  /*
    Return a shell-escaped version of the string s. The returned value is a
    string that can safely be used as one token in a shell command line, for
    cases where you cannot use a list.

    :arg s: The string whose shell-safe version is to be returned.
    :type s: `string`
    :returns: Shell-escaped version of string `s`.
    :rtype: `string`
  */
  proc quote(s:string) {
    if(s == '') {
      return '""';
    }
    if(!_find_unsafe.search(s)) {
      return s;
    }
    return "'" + s.replace("'", "'\"'\"'") + "'";
  }

  /*
    Concatenates tokens from list or array of strings and returns equivalent
    string. The functionality is the inverse of `split()` function.

    :arg x: The list of tokens to be concatenated.
    :type x: `ref` (specifically tuple of strings or array of strings)
    :returns: String with list elements as tokens in same order.
    :rtype: `string`
  */
  proc join(const ref x): string where (isArray(x) || isTuple(x)) {
    if x.size == 0 then return "";
    if(isTuple(x) && (!isHomogeneousTuple(x) || !isString(x[0]))) then
      compilerError("join() only takes homogeneous tuple of strings " +
                      "or array of strings");
    if(isArray(x) && !isString(x.first))  then
      compilerError("join() only takes homogeneous tuple of strings " +
                      "or array of strings");
    if x.size == 1 {
      if isArray(x) then return x.first;
      else  return x[0];
    }
    var res = "";
    var beginning: bool = false;
    for ele in x do {
      if beginning then res += " ";
      else  beginning = true;
      if (!_find_unsafe.search(ele)) {
        res += ele;
      }
      else {
        res += quote(ele);
      }
    }
    return res;
  }

  /*
    Overload for `join()` function to handle multiple strings as arguments.
  */
  proc join(args:string ...?n) {
    return join(args);
  }
}
