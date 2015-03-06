//
// A simple infix to postfix expression converter.
// This converts infix expressions involving single digit integers, '+', '-',
// '*', '/' and parenthesis. It doesn't error check much, and on malformed
// infix strings it will likely have lots of trouble.
//


config const infix = "3*((1+3-2)/4)+7";

record Stack {
  class node {
    var element: string;
    var next: node;
  }

  var head: node;
  proc push(s: string) {
    if head == nil {
      head = new node(s);
    } else {
      head = new node(s, head);
    }
  }
  proc pop() {
    if head == nil {
      return "";
    } else {
      var e = head.element;
      head = head.next;
      return e;
    }
  }
  proc peek() {
    if head == nil then
      return "";
    else
      return head.element;
  }
}

proc infix2postfix(str: string) {
  var operDom: domain(string);
  operDom += "+"; operDom += "-";
  operDom += "*"; operDom += "/";
  operDom += "(";

  const prec: [operDom] int =
    [ "+" => 0, "-" => 0, "*" => 1, "/" => 1, "(" => -1 ];

  var post: string = "";
  var stack: Stack;

  for i in 1..(str.length) {
    var ch: string;
    var s = str.substring(i);
    select s {
      when " ", "\t" do { /* ignore whitespace */ }
      when "+", "-", "*", "/" {
        if stack.peek() == "" then
          stack.push(s);
        else {
          while (stack.peek() != "" && prec(stack.peek()) >= prec(s)) {
            post += stack.pop();
          }
          stack.push(s);
        }
      }
      when "(" {
        stack.push("(");
      }
      when ")" {
        do {
          var next = stack.pop();
          post += next;
        } while stack.peek() != "" && stack.peek() != "(";
        assert(stack.peek() != "");
        stack.pop();
      }
      when "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" {
        post += s;
      }
      otherwise {
        halt("Bad infix string value: ", s);
      }
    }
  }
  while (stack.peek() != "") do
    post += stack.pop();
  return post;
}


proc postfixEval(str: string) {
  var stack: Stack;
  for i in 1..(str.length) {
    var s: string = str.substring(i);
    select s {
      when "0","1","2","3","4","5","6","7","8","9" {
        stack.push(s);
      }
      when "+" {
        var t1, t2: real;
        t2 = stack.pop():real;
        t1 = stack.pop():real;
        stack.push((t1 + t2):string);
      }
      when "-" {
        var t1, t2: real;
        t2 = stack.pop():real;
        t1 = stack.pop():real;
        stack.push((t1 - t2):string);
      }
      when "*" {
        var t1, t2: real;
        t2 = stack.pop():real;
        t1 = stack.pop():real;
        stack.push((t1 * t2):string);
      }
      when "/" {
        var t1, t2: real;
        t2 = stack.pop():real;
        t1 = stack.pop():real;
        stack.push((t1 / t2):string);
      }
      otherwise {
        assert(false);
      }
    }
  }
  return stack.pop():real;
}

var postfix = infix2postfix(infix);
var result  = postfixEval(postfix);
writeln("infix  : ", infix);
writeln("postfix: ", postfix);
writeln("result : ", result); 

