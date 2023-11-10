all:udpServer udpClient
udpServer:udpServer.cc
	g++ -o $@ $^ -std=c++11
udpClient:udpClient.cc
	g++ -o $@ $^ -std=c++11 -lpthread
.PHONY:clean
clean:
	rm -rf udpServer udpClient