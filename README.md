# TCP Multicast Chat
Client and server for client server written in C

Distributed chat service. Users (clients) are allowed to connect to the server and specify a nickname to be used as an alias. Duplicates are not allowed. After connecting, all messages sent by any user will be broadcasted to all other users' currently connected. In addition, the server will inform
all clients whenever a new user logs in. Chat directory is also implemented and stored in database.txt. 
