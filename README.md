# TOX Client

This is the main README.md file for the project. I have saved the initial [`README.md`](resources/README.md) under resources as a tracibility of work item. 

## Basic functionality

`tox_clinent` is a simple command line utility designed to `pull`, `get` and `rate` toxes from the tox service described in [public-api.md](texttok/public-api.md). The program saves toxes in a vector of classes that represent the objects (Implementated in [Tox.h](lib/src/Tox.h) and [Tox.cpp](lib/src/Tox.cpp))

The system implements a simple class to deal with sockets (implemented in [Socket.h](lib/src/Socket.h) and [Socket.cpp](lib/src/Socket.cpp)). This creates a wrapper around client side socket functions such as [socket](https://man7.org/linux/man-pages/man2/socket.2.html), [connect](https://man7.org/linux/man-pages/man2/connect.2.html), [read](https://man7.org/linux/man-pages/man2/read.2.html), and [write](https://man7.org/linux/man-pages/man2/write.2.html) as well as providing [DNS name resolution](https://en.wikipedia.org/wiki/Domain_Name_System) in order to get the IP address for the named service.


## Project structure

The project is seperated into `library code`, `test code`, and `application code`. All library code lives in the `lib` directory under `src`. All test code is in `test/src` and all application code is in `src` in the main directory. 

The library, `libtox_client.so`, which contains the `Tox` and `Socket` code is built as a [shared library](https://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html) and linked with the load path embedded into the binarry so that it can be loaded without having to modify the environment through `LD_LIBARY_PATH`. This is done in the link phase through the flags `-Wl,-rpath,$(LNK_PATH)` where LINK_PATH defines the path to the build directory for the `.so` file.

## Building and project dependancies

The project depends on [Google Test](https://github.com/google/googletest) which can e installed with `sudo apt install libgtest-dev`

To build the program call `make`

To build and run the unit tests call `make test`

## Functionality and how to use

To run the program, call `./build/tox_client`. This will give you a menu

```text
menu 
1. Get next Tox
2. Print collected Toxes
3. Upvote Tox
4. Downvote Tox
5. Save toxes to file
6. Load toxes from file
7. Exit
Enter choice: 
```

To invoke the action, type the number and hit return.

### Get the next tox

This will invoke the API item `Client request: Get recent Tox ID`. As this does not guarenty that only new tox will be retrieved, it might result in an existing tox being pulled. If this happens you will be notified by the programs output.

### Print collected Toxes

This does what it says. It prints the tox with data concerning the tox bellow.

```text
`. `. `. `. `. `, s @ @ s  `. `. `. `. `. `.  
. `. `. `. `. d @ @ @ @ '       `. `. `. `. `
 `. `. `. `.@ @ @ @ @ P           `. `. `. `.
`. `. `. `d @ @ @ @ @             . `. `. `.  
. `. `. `.@ @ @ @ @ *              `. `. `. `
 `. `. `: @ @ @ @ @                  `. `. `.
`. `. `.: @ @ @ @ '                 `. `. `.  
. `. `. `.@ P '   " b               . `. `. `
 `. `. `. Y         Y s            . `. `. `.
`. `. `. `l         j "             `. `. `.  
. `. `. `.@       , Y              `. `. `. `
 `. `. `. @ b * * @ '                `. `. `.
`. `. `. `@ '     @                 `. `. `.  
. `. `. `." .   , @ b .            `. `. `. `
 `. `. `. `.@   Y @ @ P           `. `. `. `.
`. `. `. `. @   : @ @ '          `. `. `. `.  
. `. `. `. `Y . ` @ . .          . `. `. `. `
 `. `. `. `.: l d @ * '           `. `. `. `.
`. `. `. `. : @ @ @              `. `. `. `.  
. `. `. `. `. Y @ @ .         . `. `. `. `. `
 `. `. `. `. `. " @ @        . `. `. `. `. `.
`. `. `. `. `. `. `."    . `. `. `. lbs 10-21 

 

Rating: -1 👎
Tox ID: dc49fe95
Currently held Rating: -1
```

### Upvote tox

This will upvote the selected tox. You will see 

```text
Enter choice: 3
Please end the ID of the Tox to upvote: 
```

You then need to supply the id of the tox to upvote, i.e. `dc49fe95`

### Downvote tox 

Does the literal oposite of Upvote tox

### Save toxes to file

This will serialise all currently heald tox to a file that you specify

```text
Enter choice: 5
Please supply a file name: 
```

i.e. `test.bin`

### Load toxes from file

Loads toxes from the supplied file


### EXit

EXits the probgram

## TODO 

1. It would be nice to know if there is an API to pull the ratting for the tox?
2. Tidy up the way in which data is sent and retrieved from the server. Currently this happens for convienience through sinlge calls to `write_to_server` and `read_from_Server` in the methods of the `Tox` class that `get` `pull` and `vote` on toxes. Ideally, for efficiency and programatic clenliness, the data should be moved into a single call for each protocol item anbd packed into a structure that is written to and read from the socket.