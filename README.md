# TOX Client

`tox_clinent` is a simple command line utility designed to `pull`, `get` and `rate` toxes from the tox service described in [public-api.md](texttok/public-api.md). The program saves toxes in a vector of classes that represent the objects (Implementated in [Tox.h](lib/src/Tox.h) and [Tox.cpp](lib/src/Tox.cpp))

The system implements a simple class to deal with sockets (implemented in [Socket.h](lib/src/Socket.h) and [Socket.cpp](lib/src/Socket.cpp)). This creates a wrapper around client side socket functions such as [socket](https://man7.org/linux/man-pages/man2/socket.2.html), [connect](https://man7.org/linux/man-pages/man2/connect.2.html), [read](https://man7.org/linux/man-pages/man2/read.2.html), and [write](https://man7.org/linux/man-pages/man2/write.2.html) as well as providing [DNS name resolution](https://en.wikipedia.org/wiki/Domain_Name_System) in order to get the IP address for the named service.



## TODO 