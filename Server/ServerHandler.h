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
                        std::make_shared<Session>(std::move(socket))->start();
                    }
                    connectionListener();
                }
            );
        }

    private:
        asio::ip::tcp::acceptor servAcceptor;

};