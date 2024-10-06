# Requirements
- C++ 17
- cmake
- Boost.Asio
- nlohmann JSON library

# Instalation
First, you need to go to the working directory and install this repository via HTTPS:

```bash
$ git clone https://github.com/falafe1y/TcpJsonClientServer.git
```

or SSH:

```sh
$ git clone git@github.com:falafe1y/TcpJsonClientServer.git
```

Go to the directory TcpJsonClientServer and install [nlohmann library](https://github.com/nlohmann/json): 

```bash
$ cd TcpJsonClientServer/
$ git clone git@github.com:nlohmann/json.git
```

If you did everything correctly, the structure of your project should be as follows:

```
- client.cpp
- CMakeLists.txt
- json/
- README.md
- server.cpp
```

Next, run this list of commands:

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```

If everything is done correctly, two executable files will appear in the build directory: **client** and **server**. You need to open two terminals and run the server on the first terminal, and the client on the second

```sh
$ ./server
$ ./client
```
