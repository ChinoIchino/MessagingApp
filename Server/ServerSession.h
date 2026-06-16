#include "Packet.h"

#include <asio.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <vector>

using asio::ip::tcp;

class ServerHandler;

class ServerSession: public std::enable_shared_from_this<ServerSession>{
    public:
        ServerSession(ServerHandler& currServer, tcp::socket socket)
            :sessionSocket(std::move(socket)),
            header(2),
            readBuff(1024),
            server(currServer){};

        void packetListener();
        
        void send(Packet* packet);
        void write(Packet* packet);
        
        
        private:
        ServerHandler& server;
        
        tcp::socket sessionSocket;
        
        std::vector<char> header;
        std::vector<char> readBuff;
        std::string decodedPacket[2];
        
        int handlePacketDecoding(std::string* toSaveTo, unsigned char* toDecode);
        bool handleAccountConnection(std::string* connectionTryInformation);
        
        // void read();
};