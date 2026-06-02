#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>


/**
 * Encoding of packets:
 *      Login packet: [uint8_t type][uint8_t sizeOfUsername][std::string usernameInformation][uint8_t sizeOfPassword][std::string passwordInformation]
 *      Message packet: [uint_8 type][uint16_t sizeOfInformation][std::string stringInformation]
 */
class Packet{
    public:
        enum PacketType{
                LOGIN,
                MESSAGE,
                JOIN_GROUP
        };

        Packet(PacketType type, std::string* information){
            this->type = type;
            this->handlePacketEncoding(information);
        };

        unsigned char* getContainer(){
            return this->container.data();
        }
        int getSizeOfContainer() const{
            return this->container.size();
        }

        void handlePacketEncoding(std::string* information){
            switch(this->type){
                case PacketType::LOGIN:{
                    if(information->size() < 2){
                        std::cout << "ERROR Packet::handlePacketEncoding : std::string* information doesn't contains enough attributs" << std::endl;
                        return;
                    }
                    // Write the id of the packet, the size of the username, the username, the size of the password, the password
                    writeUint8(0);

                    writeUint8(information[0].size());
                    writeStringInPacket(information[0]);

                    writeUint8(information[1].size());
                    writeStringInPacket(information[1]);

                    break;
                }
                case PacketType::MESSAGE:{
                    // Write the id of the packet, the size of the message, and the message
                    writeUint8(1);

                    writeUint16(information[0].size());
                    writeStringInPacket(information[0]);

                    break;
                }
                case PacketType::JOIN_GROUP:{
                    // Id of packet
                    writeUint8(2);
                    // Is he creating a lobby
                    writeUint8(information[0].at(0));
                    // Id of group
                    writeUint16(std::stoi(information[1]));
                    // Size of password
                    writeUint8(information[2].size());
                    // Password of lobby if information[0] == 1, else its a password try
                    writeStringInPacket(information[2]);

                }
                default:{
                    break;
                }
            }
        }

        // std::string getInformation(){
        //     return this->readInformationInPacket();
        // };

        void printPacket(){
            // Save the old cursor, and put it at the start 
            int oldCursor = this->cursor;
            this->cursor = 0;

            switch(this->readUint8()){
                case PacketType::LOGIN:{
                    std::cout 
                    << "Login Packet:\n  Username : "
                    << this->readInformationInPacket(this->readUint8())
                    << "\n  Password : "
                    << this->readInformationInPacket(this->readUint8())
                    << std::endl;

                    // Set it back where it was before the print
                    this->cursor = oldCursor;

                    break;
                }
                case PacketType::MESSAGE:{
                    std::cout 
                    << "Message Packet:\n  Contain the message : "
                    << this->readInformationInPacket(this->readUint16())
                    << std::endl;

                    // Set it back where it was before the print
                    this->cursor = oldCursor;

                    break;
                }
                default:{
                    std::cout << "ERROR Packet::printPacket : Unknown packet type" << std::endl;
                    break;
                }
            }
        }

    private:
        PacketType type;
        std::vector<unsigned char> container;
        unsigned int cursor = 0;

        void writeUint8(uint8_t value){
            this->container.push_back(value);
        }
        void writeUint16(uint16_t value){
            this->container.push_back((value >> 8) & 0xFF);
            this->container.push_back(value & 0xFF);
        }
        void writeStringInPacket(std::string value){
            this->container.insert(this->container.end(), value.begin(), value.end());
        }

        int readIntInPacket(){
            return int((unsigned char)(this->container.at(this->cursor++)) << 24 |
                (unsigned char)(this->container.at(this->cursor++)) << 16 |
                (unsigned char)(this->container.at(this->cursor++)) << 8 |
                (unsigned char)(this->container.at(this->cursor++)));
        }
        uint8_t readUint8(){
            return this->container.at(this->cursor++);
        }
        uint16_t readUint16(){
            return uint16_t((unsigned char)(this->container.at(this->cursor++)) << 8 |
                (unsigned char)(this->container.at(this->cursor++)));
        }
        std::string readInformationInPacket(uint16_t sizeToRead){
            std::string toReturn(this->container.begin() + this->cursor, this->container.begin() + this->cursor + sizeToRead);
            this->cursor += sizeToRead;

            return toReturn;
        }
};