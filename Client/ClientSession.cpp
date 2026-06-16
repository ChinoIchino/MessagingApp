// #include "ServerHandler.h"
#include "ClientSession.h"

int ClientSession::handlePacketDecoding(std::string* toSaveTo, unsigned char* toDecode){
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

bool ClientSession::handleAccountConnection(std::string* connectionTryInformation){
            //TODO verify if the account exist on the database
            std::cout << "Trying a connection" << std::endl;
            return true;
}

void ClientSession::packetListener(){
    std::cout << "Starting to listening to a session // Server folder" << std::endl;
    
    auto self = shared_from_this();

    asio::async_read(
        this->sessionSocket,
        asio::buffer(this->readBuff),
        [this, self](std::error_code ec, std::size_t length){
            if(!ec){
                std::cout << "Received a packet. Starting again the packetListener function" << std::endl;
            }
            this->packetListener();
        }
    ); 
};

void ClientSession::send(Packet* packet){
    auto self(shared_from_this());
    
    // this->server.broadcast(shared_from_this(), packet);
    write(packet);
}

void ClientSession::write(Packet* packet){
    auto self = shared_from_this();
    
    std::cout << "Got to the write function of ClientSession" << std::endl;
    packet->printPacket();

    // packet. << " // size " << packet->getSizeOfContainer() << std::endl;
    
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

void ClientSession::read(){
    auto self(shared_from_this());

    asio::async_read(
        this->sessionSocket,
        asio::buffer(this->readBuff),
        [this, self](std::error_code ec, std::size_t length){
            if(!ec){
                std::cout << "Got in the buffer in read function: " << this->readBuff.data() << std::endl;

                switch(handlePacketDecoding(this->decodedPacket, (unsigned char*) this->readBuff.data())){
                    case 0:{
                        std::cout << "Session: Login Packet :\n  Username : " << this->decodedPacket[0] << "\n  Password : " << this->decodedPacket[1] << std::endl;
                        break;
                    }
                    case 1:{
                        std::cout << "Session: Message Packet :\n  Message : " << this->decodedPacket[0] << std::endl;
                        break;
                    }
                    default:{
                        std::cout << "ERROR Session::read : handlePacketDecoding returned a error type or a non handled one" << std::endl;
                        break;
                    }
                }
                
                this->read();
            }else{
                std::cout << "ERROR Session::read(): " << ec.message() << std::endl;
            }
        }
    );
}