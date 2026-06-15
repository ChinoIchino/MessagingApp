#include "ServerSession.h"

#include <ctime>
#include <iostream>
#include <string>
#include <thread>
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
            std::cout << "starting a new connectionListener" << std::endl;
            this->servAcceptor.async_accept(
                [this](std::error_code ec, tcp::socket socket){
                    if(!ec){
                        std::cout << "Client connected to server via the ip : " << socket.remote_endpoint().address().to_string() << std::endl;

                        std::shared_ptr<ServerSession> newClient = std::make_shared<ServerSession>(*this, std::move(socket));
                        this->clientsConnected.push_back(newClient);

                        std::cout << "Client added to clientsConnected list // Ammount of clients " << this->clientsConnected.size() << std::endl;

                        std::cout << "About to start a packetListener" << std::endl;
                        newClient->packetListener();
                    }
                    connectionListener();
                }
            );
        }
        
        // void broadcast(std::shared_ptr<Session> sender, Packet* packet){
        //     for(std::shared_ptr<Session>& client: this->clients){
        //         if(client != sender){
        //             client->write(packet);
        //         }
        //     }
        // }
        void handleAndBroadcast(std::vector<char> buffer){

        }

        bool handleAccountConnection(std::string* connectionTryInformation){
            //TODO verify if the account exist on the database
            std::cout << "Trying a connection" << std::endl;
            return true;
        }

        // void read(std::shared_ptr<ServerSession> sessionToListen, tcp::socket socketOfSession){
            
        // }

        
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

        void removeConnection(std::shared_ptr<ServerSession> connectionToRemove){
            this->clientsConnected.erase(
                std::remove(this->clientsConnected.begin(), this->clientsConnected.end(), connectionToRemove),
                this->clientsConnected.end()
            );

            std::cout << "Removed a connection // Ammount of clients left " << this->clientsConnected.size() << std::endl;
        }

        
    private:
        asio::ip::tcp::acceptor servAcceptor;
        std::vector<std::shared_ptr<ServerSession>> clientsConnected;
        std::vector<std::vector<std::shared_ptr<ServerSession>>> clientsInGroup;

        bool isRunning = true;
};