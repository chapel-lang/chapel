## Channel Module

A channel is a FIFO data structure used to communicate between different Chapel
tasks. You can learn more about how go-channels work by following this
[link](https://gobyexample.com/channels).
Since the operations on the channel are blocking by default, it enables the user
to send values from one task to another in a synchronized manner.
Here is a simple example in Go.

```
func main() {
	// make a channel for string values
	messages := make(chan string)

	// new task to send a value to the channel
	// it will wait until other task receives
	go func() { messages <- "ping" }()

	// receive the sent value
	msg := <-messages
	fmt.Println(msg)
}
```

The module will have a generic `Channel` class (or record) to store the channel
buffer and support operations on them.

* The main functions that the module should have

	* `init(type eltType, size = 0)` : Specify the buffer data type and buffer size
	for creating a Channel.

	* `send(value : eltType)` : This function waits for a space in the buffer
	and	sends the `value` to the channel.

	* `recv(out value : eltType) : bool` : This function waits until there is
	something in the buffer and stores the received entity in `value`. It also
	returns the status (`false` if the channel is closed and its buffer is empty).

	* `close()` : This function is used to close the channel such that no more
	values will be sent to it.

* `iter these()` : An iterator to receive the values sent to the channel until
it is closed.

For e.g., the above Go code in chapel would look like

```
var messages = new Channel(string);
begin {
	messages.send("ping");
}

var msg : string;
messages.recv(msg);
writeln(msg);
```

### Select statement

This statement lets you wait for multiple channel operations and executes if
any one of them is ready for communication. `select`statements in Go are
specifically designed to work with channels.

Some key points of `select` are:

	* `select` statements are commonly used in Go code when working with channels.
	* The operations are atomic, i.e., checking a channel and performing `recv`
		or `send` on it is considered as one operation.
	* The cases are evaluated in a random order to prevent starvation.

For e.g.
```
// Select syntax in Go to wait on any one of the two channels for receiving
select {
	case msg1 := <-chan1 :
		fmt.Println("Received", msg1)
	case msg2 := <-chan2 :
		fmt.Println("Received", msg2)
}
```

```
// Select statements can be used to implement try-send or try-recv
// default case is executed when none of the other cases are ready yet.
select {
	case val := <-chan1 :
		fmt.Println("Received", val);
	default :
		fmt.Println("Channel not ready");
}
```

### Some important questions related to the module-

* Should this module be standard or package ?

* What should be the name of the module ? Some names in other languages
	* `chan` in Golang
	* `crossbeam` in Rust

* What should be the name of the channel data type? Should it be record or
class ?
	* Channel should always be passed as a reference therfore we could make
	it a class or a record-wrapped class.

* What should be the syntax of Channel `select` statements in Chapel ?
Should we use Chapel's `select` statement for that? Some semantic differences
between `select` in Chapel and Channels.

	* Chapel's select is a conditional statement which is used to evaluate
	cases against one statement specified after `select` keyword whereas
	`select` in Channels work with multiple Channel operations.

	* `select` for Channels can be blocking or non-blocking while `select`
	in Chapel is always non-blocking.

	* A Go-style `select` statement has no argument but a Chapel one does have an
	argument. As a result these could be easily distinguished at the syntax level.

* Should operators be used for `send` and `recv` routines ? The operators in Go
 behave differently in case of receive operation
	* `val := <-chan` will only return receive value.
	* `val, ok := <-chan` will store received value in `val` and status in `ok`.
Does operators in Chapel channels need to have this kind of behaviour?
