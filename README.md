# rps_game_server
Sever part of RPS game, one of the online games.


RPS server is fully developed and operational at Linux. It is designed and implemented based on Event-Driven architecture. The whole Framework(half-sync/half-async) is built on foundations like event reactor、protocol stack、thread-safe queue and timer, which are provided for further requirements. All these infrastructures are well decoupled with business layer. Since the epoll---the most efficient way of I/O multiplexing, is utilized inside event reactor, this sever is supposedly able to support 10K concurrent connections. Please be noted that this program is implemented with lots of C++11 features and functionalities.
There are several advantages and drawbacks for current version.

Advantage:
1)	High performance with little resource.
2)	Excellent expandability and extensibility
3)	Well portable. Most of the code could be compiled in any compilers as long as they support C++11, even from X84 system to X64.
4)	 Excellent makefiles are flexible for any source file add/delete/update.

Drawback:
1)	Event log and debug log still need more time to reconstruct.
2)	Fault tolerance is not so good while many auxiliary functions yet to be developed.
3)	Thread pool and memory pool yet to be developed.
4)	Help manual yet to be added.
5)	Configuration file/function yet to be added.
6)	No watchdog.
7)	Thread-safe hash map/map yet to be developed.
8)	Free-lock data structures yet to be developed.

Usage:
1)	../rps_game_server/src/, You can type “make” to start build.
2)	../rps_game_server/build/bin, you can start server via ./rps, by default, the IP of eth0 and port 8088 is used. Aslo you can start server /rps #ip #port.
3)	../rps_game_server/build/log, you can check the event log.
