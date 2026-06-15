#include "ServerHandler.h"
// #include "ServerSession.h"

int ServerSession::handlePacketDecoding(std::string* toSaveTo, unsigned char* toDecode){
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

bool ServerSession::handleAccountConnection(std::string* connectionTryInformation){
            //TODO verify if the account exist on the database
            std::cout << "Trying a connection" << std::endl;
            return true;
}

void ServerSession::packetListener(){
    // std::cout << "Starting to listening to a session // Server folder" << std::endl;

    // asio::async_read(
    this->sessionSocket.async_read_some(
        // this->sessionSocket,
        asio::buffer(this->readBuff),
        [this](std::error_code ec, std::size_t length){
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
                
                this->packetListener();
            }
            else{
                std::cout << "PacketListener received a error code " << ec.message() << std::endl;
                
                auto self(shared_from_this());
                this->server.removeConnection(self);
                return;
            }
        }
    ); 
};

void ServerSession::send(Packet* packet){
    auto self(shared_from_this());
    
    // this->server.broadcast(shared_from_this(), packet);
    write(packet);
}

void ServerSession::write(Packet* packet){
    auto self(shared_from_this());

    std::cout << "ServerSession::write function got called" << std::endl;
    
    asio::async_write(
        sessionSocket,
        asio::buffer(packet->getContainer(), packet->getSizeOfContainer()),
        [this, self](std::error_code ec, std::size_t){
            if(!ec){
                this->packetListener();
            }
        }
    );
}

// void ServerSession::read(){
//     auto self(shared_from_this());

//     std::cout << "ServerSession::read function got called" << std::endl;

//     // asio::async_read(
//     this->sessionSocket.async_read_some(
//         // this->sessionSocket,
//         asio::buffer(this->readBuff),
//         [this, self](std::error_code ec, std::size_t length){
//             if(!ec){
//                 switch(handlePacketDecoding(this->decodedPacket, (unsigned char*) this->readBuff.data())){
//                     case 0:{
//                         std::cout << "Session: Login Packet :\n  Username : " << this->decodedPacket[0] << "\n  Password : " << this->decodedPacket[1] << std::endl;
//                         break;
//                     }
//                     case 1:{
//                         std::cout << "Session: Message Packet :\n  Message : " << this->decodedPacket[0] << std::endl;
//                         break;
//                     }
//                     default:{
//                         std::cout << "ERROR Session::read : handlePacketDecoding returned a error type or a non handled one" << std::endl;
//                         break;
//                     }
//                 }
                
//                 this->read();
//             }else{
//                 std::cout << "ERROR ServerSession::read(): " << ec.message() << std::endl;
                
//                 if(ec.value() == 1236){
//                     std::cout << "About to remove the connection" << std::endl;
//                 }
//             }
//         }
//     );

    // std::cout << "About to remove the connection" << std::endl;
    // this->server.removeConnection(self);
// }