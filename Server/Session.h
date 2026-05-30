#include "Packet.h"

#include <asio.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <vector>

using asio::ip::tcp;

class ServerHandler;

class Session: public std::enable_shared_from_this<Session>{
    public:
        Session(ServerHandler& currServer, tcp::socket socket)
            :sessionSocket(std::move(socket)),
            readBuff(1024),
            server(currServer){};

        void start(){
            read();
            Packet* testPacket = new Packet(Packet::MESSAGE, new std::string[1]{"Got this random information."});
            send(testPacket);
        };

        void send(Packet* packet){
            auto self(shared_from_this());
            
            this->server.broadcast(shared_from_this(), packet);

            asio::async_write(
                sessionSocket,
                asio::buffer(packet->getContainer(), packet->getSizeOfContainer()),
                [this, self](std::error_code ec, std::size_t){
                    if(!ec){
                        read();
                    }
                }
            );
        }

    private:
        ServerHandler& server;

        tcp::socket sessionSocket;
        
        const short int READ_BUFFER_SIZE = 1024;
        std::vector<char> readBuff;
        
        void read(){
            auto self(shared_from_this());

            asio::async_read(
                this->sessionSocket,
                asio::buffer(this->readBuff),
                [this, self](std::error_code ec, std::size_t length){
                    if(!ec){
                        std::string messageReceived(this->readBuff.data(), length);
                        std::cout << "Session: Got the message : " << messageReceived << std::endl;
                        
                        read();
                    }else{
                        std::cout << "ERROR Session::read(): " << ec.message() << std::endl;
                    }
                }
            );
        }
};