#include <stdio.h>
#include <iostream>
#include <vector>

class Packet{
    public:
        enum PacketType{
                LOGIN,
                MESSAGE,
                LOGOUT
        };

        Packet(PacketType type, std::string information){
            this->type = type;
            this->handlePacketEncoding(information);
        };

        void handlePacketEncoding(std::string information){
            switch(this->type){
            case PacketType::LOGIN:{
                //TODO encoding in login packet
                break;
            }
            case PacketType::MESSAGE:{
                // Write the id of the packet, the size of the message, and the message
                writeInPacket<std::uint8_t>(2);
                writeInPacket<std::uint16_t>(information.size());
                writeInPacket<std::string>(information);
                break;
            }
            case PacketType::LOGOUT:{
                
                break;
            }
            default:
                break;
            }
        }
        void handlePacketDecoding(){
            switch(this->type){
            case PacketType::LOGIN:{
                //TODO encoding in login packet
                break;
            }
            case PacketType::MESSAGE:{
                // Write the id of the packet, the size of the message, and the message
                read()
                break;
            }
            case PacketType::LOGOUT:{
                
                break;
            }
            default:
                break;
            }
        }

        std::string getInformation() const{
            int type = this->readInPacket<int>();
            int sizeOfInformation = this->readInPacket<int>();
        };

        void printPacket(){
            std::cout 
            << "Packet:\n  Size = " 
            << this->readPacketSize()
            << "\n  Container: " 
            << this->container.data()
            << std::endl;
        }

        int readPacketSize(){
            int decodedInt;
            memcpy(&decodedInt, this->container.data(), sizeof(int));

            return decodedInt;
        };

    private:
        PacketType type;
        std::vector<unsigned char> container;
        unsigned int cursor = 0;

        template<typename T>
        void writeInPacket(const T& value){
            const unsigned char* itemPointer = reinterpret_cast<const unsigned char*>(&value);
        
            this->container.insert(this->container.end(), itemPointer, itemPointer + sizeof(T));
        }

        template<typename T>
        T readInPacket(){
            T itemPointer = reinterpret_cast<T>(this->container.at(this->cursor));
            this->cursor += sizeof(T);

            return itemPointer;
        }
};