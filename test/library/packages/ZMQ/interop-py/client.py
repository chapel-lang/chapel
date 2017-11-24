# client.py
import zmq

context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

for request in range(10):
    message = "Hello %i from Python" % request
    print("[Python] Sending request: %s" % message)
    socket.send_string(message)
    message = socket.recv_string()
    print("[Python] Received response: %s" % message)
