/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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
Chapel's version of Shlex (pyhthon) implemented here:
<https://github.com/python/cpython/blob/3.7/Lib/shlex.py>
This module provides support for split, join and quote operations.
*/

pragma "no doc"
module Shlex {
  private use List;
  private use Regex;
  class shlex{
    var instream: string;
    var infile: string;
    var posix: bool;
    var lineno: int;
    var debug: int;
    var commenters: string;
    var wordchars: string;
    var whitespace: string;
    var whitespace_split: bool;
    var quotes: string;
    var escapse: string;
    var escapsedquotes: string;
    var state: string;
    var token: string;
    var tokindex: int;
    var punctuation_chars: string;
    var pushback: list(string);
    var _pushback_chars: list(string);
    var custom_punctuation_chars: string;

    //
    // Initializes the required variables of class Shlex
    // Instream is the string to be parsed and is the only required argument
    //
    proc init(instream:string, posix: bool = false,
                punctuation_chars: bool = false, commenters: string = '#',
                  custom_punctuation_chars: string = '') {
      this.instream=instream.strip(trailing=true);
      this.posix=posix;
      this.lineno=1;
      this.debug=0;
      this.commenters=commenters;
      this.wordchars="abcdfeghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
      this.wordchars+="0123456789_";
      if posix{
        this.wordchars+="ßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýþÿÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐ";
        this.wordchars+="ÑÒÓÔÕÖØÙÚÛÜÝÞ";
      }
      this.whitespace= " \t\n\r";
      this.whitespace_split=false;
      this.quotes="\"'";
      this.escapse="\\";
      this.escapsedquotes='"';
      this.state=" ";
      this.token='';
      this.tokindex=0;
      if (punctuation_chars == false) {
        this.punctuation_chars='';
      }
      else{
        this.punctuation_chars= if custom_punctuation_chars=='' then '();<>|&'
        else custom_punctuation_chars;
        this.wordchars+='~-./?*=';
        for c in this.punctuation_chars {
          this.wordchars=this.wordchars.replace(c, '');
        }
      }
    }

    //
    // Returns the next token from the input stream
    //
    proc get_token():string{
      if !this.pushback.isEmpty() {
        var tok=this.pushback.pop(0);
        if this.debug >=1 {
          writeln("shlex: popping token %s\n", tok);
        }
        return tok;
      }
      if(this.tokindex==-1){
        return '';
      }
      var raw= try! this.read_token();
      if this.debug>=1{
        if(raw!=''){
          writeln("shlex: token="+raw);
        }
        else{
          writeln("shlex: token=EOF");
        }
      }
      return raw;
    }

    //
    // Reads next token and returns it
    //
    proc read_token() throws{
      var quoted:bool = false;
      var escapsedstate:string = ' ';
      var nextchar:string='';
      while true {
        if this.tokindex==-1 then break;
        if (this.punctuation_chars!='' && !this._pushback_chars.isEmpty()){
          nextchar=this._pushback_chars.pop();
        }
        else if(this.tokindex<this.instream.size){
          nextchar=this.instream[this.tokindex];
          this.tokindex+=1;
        }
        else{
          nextchar='';
          this.tokindex=-1;
        }
        if this.debug>=3 then writeln("shlex: in state ", this.state,
                                        "I see character: ", nextchar);
        if nextchar=='\n' then this.lineno+=1;
        if this.state==''{
          this.token = '';
          break;
        }
        else if this.state==' '{
          if nextchar=='' {
            this.state='';
            break;
          }
          else if(this.whitespace.find(nextchar)!=-1){
            if(this.debug>=2){
              writeln("shlex: I see whitespaces in whitespace state");
            }
            if (this.token!='' || (this.posix && quoted)) then break;
            else continue;
          }
          else if(this.commenters.find(nextchar)!=-1){
            while(this.tokindex<this.instream.size &&
                    this.instream[tokindex]!='\n') do
              this.tokindex+=1;
            this.lineno+=1;
          }
          else if(this.posix && this.escapse.find(nextchar)!=-1){
            escapsedstate='a';
            this.state=nextchar;
          }
          else if(this.wordchars.find(nextchar)!=-1){
            this.token=nextchar;
            this.state='a';
          }
          else if(this.punctuation_chars.find(nextchar)!=-1){
            this.token=nextchar;
            this.state='c';
          }
          else if(this.quotes.find(nextchar)!=-1){
            if !this.posix then this.token=nextchar;
            this.state=nextchar;
          }
          else if(this.whitespace_split){
            this.token=nextchar;
            this.state='a';
          }
          else{
            this.token=nextchar;
            if (this.token!='' || (this.posix && quoted)) then break;
            else continue;
          }
        }
        else if (this.quotes.find(this.state)!=-1){
          quoted=true;
          if nextchar==''{
            if this.debug>=2{
              writeln("shlex: I see EOF in quoted state");
            }
            throw new Error("No closing quotation");
          }
          if (nextchar==this.state){
            if !this.posix{
              this.token+=nextchar;
              this.state=' ';
              break;
            }
            else this.state='a';
          }
          else if(this.posix && this.escapse.find(nextchar)!=-1 &&
                    this.escapsedquotes.find(this.state)!=-1){
            escapsedstate=this.state;
            this.state=nextchar;
          }
          else{
            this.token+=nextchar;
          }
        }
        else if(this.escapse.find(this.state)!=-1){
          if(nextchar==''){
            if(this.debug>=2){
              writeln("I see EOF in escaped state");
            }
            throw new Error("No escaped character");
          }
          if(this.quotes.find(escapsedstate)!=-1 && nextchar!=this.state &&
              nextchar!=escapsedstate){
            this.token+=this.state;
          }
          this.token+=nextchar;
          this.state=escapsedstate;
        }
        else if("ac".find(this.state)!=-1){
          if nextchar==''{
            this.state='';
            break;
          }
          else if(this.whitespace.find(nextchar)!=-1){
            if this.debug>=2 then writeln("I see whitespaces in word state");
            this.state=' ';
            if (this.token!='' || (this.posix && quoted)) then break;
            else continue;
          }
          else if(this.commenters.find(nextchar)!=-1){
            while(instream[this.tokindex]!='\n') do this.tokindex+=1;
            this.lineno+=1;
            if this.posix {
              this.state=' ';
              if (this.token!='' || (this.tokindex!=-1 ||
                  (this.posix && quoted)))
              then break;
              else continue;
            }
          }
          else if(this.state=='c'){
            if(this.punctuation_chars.find(nextchar)!=-1){
              this.token+=nextchar;
            }
            else{
              if(this.whitespace.find(nextchar)==-1){
                this._pushback_chars.append(nextchar);
              }
              this.state=' ';
              break;
            }
          }
          else if(this.posix && this.quotes.find(nextchar)!=-1){
            this.state=nextchar;
          }
          else if(this.posix && this.escapse.find(nextchar)!=-1){
            escapsedstate='a';
            this.state=nextchar;
          }
          else if((this.wordchars.find(nextchar)!=-1) ||
                    (this.quotes.find(nextchar)!=-1) || this.whitespace_split)
          {
            this.token+=nextchar;
          }
          else{
            if(this.punctuation_chars!=''){
              this._pushback_chars.append(nextchar);
            }
            else{
              this.pushback.insert(0, nextchar);
            }
            if this.debug>=2{
              writeln("shlex: I see punctuation in word state");
            }
            this.state=' ';
            if(this.token!='' || (this.posix && quoted)) then break;
            else continue;
          }
        }
      }
      var result = this.token;
      this.token='';
      if(this.debug>=2){
        if result!='' {
          writeln("shlex: raw token: "+result);
        }
        else{
          writeln("shlex: raw token= EOF");
        }
      }
      return result;
    }
  }

  //
  // This is the public interface to the SHLEX.
  // It splits a string into a list of tokens in accordance to the rules
  // specified by the Shlex object.
  //
  proc split(s:string, comments:bool = false, posix:bool=true,
              punctuation_chars: bool = false,
                custom_punctuation_chars: string = "")
  {
    var commenters='';
    if comments then commenters='#';
    var lex=new shlex(s, posix=posix, punctuation_chars=(punctuation_chars || custom_punctuation_chars!=''), commenters=commenters, custom_punctuation_chars=custom_punctuation_chars);
    lex.whitespace_split=!(punctuation_chars || custom_punctuation_chars !='');
    var lst: list(string);
    var pres:string = ' ';
    while(lex.tokindex!=-1 || !lex.pushback.isEmpty()){
      pres=lex.get_token();
      if(posix || pres!=''){
        lst.append(pres);
      }
    }
    return lst;
  }

  // Regular expression for checking if a string is a token by itself.
  var _find_unsafe = try! compile("[^\\w@%+=:,./-]");

  //
  // This is the public interface to the SHLEX.
  // It returns a shell-escaped version of the string s.
  // The returned string can be safely used as a token in command-line.
  //
  proc quote(s:string){
    if(s==''){
      return '""';
    }
    if(!s.search(_find_unsafe)){
      return s;
    }
    return "'" + s.replace("'", "'\"'\"'") + "'";
  }

  //
  // This is the public interface to the SHLEX.
  // Concatenates the tokens of the list passed into a string.
  // This is the inverse of split() function above.
  //
  proc join(const ref x): string where (isArray(x) || isTuple(x)) {
    if x.size == 0 then return "";
    if(isTuple(x) && (!isHomogeneousTuple(x) || !isString(x[0]))) then
      compilerError("join() only takes homogeneous tuple of strings " +
                      "or array of strings");
    if(isArray(x) && !isString(x[x.domain.first]))  then
      compilerError("join() only takes homogeneous tuple of strings " +
                      "or array of strings");
    if x.size == 1 {
      if isArray(x) then return x[x.domain.first];
      else  return x[0];
    }
    var res="";
    var beginning: bool = false;
    for ele in x do {
      if(beginning) then res+=" ";
      else  beginning=true;
      if (!ele.search(_find_unsafe)){
        res+=ele;
      }
      else{
        res+=quote(ele);
      }
    }
    return res;
  }

  //
  // This is the public interface to the SHLEX.
  // Overloading for multiple string arguments.
  //
  proc join(args:string ...?n){
    return join(args);
  }
}
