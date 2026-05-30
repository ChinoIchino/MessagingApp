#include "Session.h"

#include <ctime>
#include <iostream>
#include <string>
#include <asio.hpp>

using asio::ip::tcp;

class ServerHandler{
    public:
        ServerHandler(asio::io_context& io, short port)
            : servAcceptor(io, tcp::endpoint(tcp::v4(), port)){
                connectionListener();
            };
        ~ServerHandler(){};

        void connectionListener(){
            this->servAcceptor.async_accept(
                [this](std::error_code ec, tcp::socket socket){
                    if(!ec){
                        std::cout << "Client connected to server" << std::endl;
                        std::shared_ptr<Session> newClient = std::make_shared<Session>(this, std::move(socket));
                        this->clients.push_back(newClient);
                        newClient->start();
                    }
                    connectionListener();
                }
            );
        }

        void broadcast(std::shared_ptr<Session> sender, Packet* packet){
            for(std::shared_ptr<Session>& client: this->clients){
                if(client != sender){
                    client->send(packet);
                }
            }
        }
        
        int handlePacketDecoding(std::string* toSaveTo, unsigned char* toDecode){
            switch(toDecode[0]){
                case 0:{
                    uint8_t usernameSize = toDecode[1];
                    uint8_t passwordSize = toDecode[usernameSize + 2];
                
                    toSaveTo[0].clear();
                    toSaveTo[0].append(std::string(reinterpret_cast<char*>(toDecode + 2), usernameSize));

                    toSaveTo[1].clear();
                    toSaveTo[1].append(std::string(reinterpret_cast<char*>(toDecode + usernameSize + 3), passwordSize));
                
                    return 0;
                }
                case 1:{
                    uint16_t messageContentSize = uint16_t(toDecode[1] << 8 | toDecode[2]);
                
                    toSaveTo[0].clear();
                    toSaveTo[0].append(std::string(reinterpret_cast<char*>(toDecode + 3), messageContentSize));
                
                    return 1;
                }
                default:{
                    break;
                }
            }
            return -1;
        }
        
    private:
        asio::ip::tcp::acceptor servAcceptor;
        std::vector<std::shared_ptr<Session>> clients;
};