// #include "ServerHandler.h"
#include "Session.h"

int Session::handlePacketDecoding(std::string* toSaveTo, unsigned char* toDecode){
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

bool Session::handleAccountConnection(std::string* connectionTryInformation){
            //TODO verify if the account exist on the database
            std::cout << "Trying a connection" << std::endl;
            return true;
}

void Session::start(){
    std::cout << "Starting to listening to a session" << std::endl;
            // auto self(shared_from_this());
            int currentPacketType;
            std::vector<char>* readBuff = new std::vector<char>(1024);
            
            bool isConnectedToAccount = false;
            std::string* connectionTryInfo = new std::string[2];
            while(!isConnectedToAccount){
                std::cout << "Waiting for a account try connection" << std::endl;

                asio::async_read(
                    this->sessionSocket,
                    asio::buffer(*readBuff),
                    [readBuff, connectionTryInfo](std::error_code ec, std::size_t length){
                        if(!ec){
                            uint8_t usernameSize = readBuff->at(1);
                            uint8_t passwordSize = readBuff->at(usernameSize + (uint8_t) 2);
                        
                            connectionTryInfo[0].clear();
                            connectionTryInfo[0].append(std::string(reinterpret_cast<char*>(readBuff + 2), usernameSize));

                            connectionTryInfo[1].clear();
                            connectionTryInfo[1].append(std::string(reinterpret_cast<char*>(readBuff + usernameSize + 3), passwordSize));
                        }
                    }
                );

                isConnectedToAccount = handleAccountConnection(connectionTryInfo);
            }
            std::cout << "Connection to account established. Waiting for a group packet..." << std::endl;

            asio::async_read(
                this->sessionSocket,
                asio::buffer(*readBuff),
                [this](std::error_code ec, std::size_t length){
                    if(!ec){
                        std::cout << "Received a group packet" << std::endl;
                    }
                }
            );
            asio::async_read(
                this->sessionSocket,
                asio::buffer(*readBuff),
                [this](std::error_code ec, std::size_t length){
                    if(!ec){
                        // handleAndBroadcast();
                    }
                }
            );
    // this->read();
    // Packet* testPacket = new Packet(Packet::MESSAGE, new std::string[1]{"Got this random information to decode."});
    // this->send(testPacket);
};

void Session::send(Packet* packet){
    auto self(shared_from_this());
    
    // this->server.broadcast(shared_from_this(), packet);
    write(packet);
}

void Session::write(Packet* packet){
    auto self(shared_from_this());
    
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

void Session::read(){
    auto self(shared_from_this());

    asio::async_read(
        this->sessionSocket,
        asio::buffer(this->readBuff),
        [this, self](std::error_code ec, std::size_t length){
            if(!ec){
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