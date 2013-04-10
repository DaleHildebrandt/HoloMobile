###TestSockets README###

##Description
This is a VC++ solution created in Visual Studio 2010. It provides a set of 4 simple projects that can be used to test a socket connection independent of anything else and also to see what messages are being passed over the socket. Each project is a different pairing of socket type and usage:

Server & Sender
Server & Reciever
Client & Sender
Client & Receiver

Receiver projects will print out any input they get, senders will constantly print out the output line they have.

##Usage
Choose the pairing that is completely opposite to the type and usage of the socket in your program.

Remember to set the IP Address in the client socket to the server's IP Address.


##Notes
This code is heavily based on the sample socket code that can be found on MSDN.
