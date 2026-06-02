#include "ServerHandler.h"

#include <iostream>

int main(){
    try{
        asio::io_context io;

        ServerHandler server(io, 5544);

        io.run();
    }catch(std::exception& e){
        std::cout << "ERROR serverMain : " << e.what() << std::endl;
    }

    return 0;
}