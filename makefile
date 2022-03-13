
GTKLINKFLAGS=$(shell pkg-config --libs gtkmm-3.0)
GTKCOMPILEFLAGS=$(shell pkg-config --cflags gtkmm-3.0)

CXXFLAGS+= -DASIO_STANDALONE -Wall -O0 -g -std=c++11
CPPFLAGS+= ${GTKCOMPILEFLAGS} -I./include -I./asio-1.12.2/include -I./src -Wno-deprecated-declarations
LDLIBS+= -lpthread


TARGETS=asio-1.12.2 chat_client chat_server automated_tests

all:${TARGETS}

asio-1.12.2:
	tar xzf asio-1.12.2.tar.gz
	
automated_tests:./src/automated_tests.cpp ./src/game.cpp ./src/hand.cpp ./src/deck.cpp ./src/card.cpp
	${CXX} -o $@ $^  ${CXXFLAGS} ${CPPFLAGS}
	
chat_client:./src/chat_client.cpp  ./src/player.cpp ./src/game.cpp ./src/hand.cpp ./src/deck.cpp ./src/card.cpp
	${CXX} -o $@ $^  ${CXXFLAGS} ${CPPFLAGS} ${LDLIBS} ${GTKLINKFLAGS}

chat_server:./src/chat_server.cpp  ./src/player.cpp ./src/game.cpp ./src/hand.cpp ./src/deck.cpp ./src/card.cpp
	${CXX} -o $@ $^  ${CXXFLAGS} ${CPPFLAGS} ${LDLIBS} ${GTKLINKFLAGS}
	
clean:
	-rm -rf asio-1.12.2
	-rm -f chat_client
	-rm -f chat_server
	-rm -f automated_tests
