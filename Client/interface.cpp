#define SDL_MAIN_HANDLED

#include "GuiElements/GuiElements.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <asio.hpp>
#include <iostream>

// TODO make a function that connect to a database of users
void connectToAccount(void* arg){
    GuiElements* gui = (GuiElements*)(arg);
    gui->changeRender(GuiElements::GuiGroup::MAINMENU_GUI_GROUP);
    
    // std::string username = gui->getTextFieldList()[0]->getTextContainer();

    // std::cout 
    //     << "collectTextFieldInformation: Username: " << gui->getTextFieldList()[0]->getTextContainer() 
    //     << " // Password: " << gui->getTextFieldList()[1]->getTextContainer() 
    // << std::endl;
}

void sendToMainMenu(void* arg){
    GuiElements* gui = (GuiElements*)(arg);
    gui->changeRender(GuiElements::GuiGroup::MAINMENU_GUI_GROUP);
}

void sendToLoginMenu(void* arg){
    GuiElements* gui = (GuiElements*)(arg);
    gui->changeRender(GuiElements::GuiGroup::LOGIN_GUI_GROUP);
}
void sendToCreateServerMenu(void* arg){
    GuiElements* gui = (GuiElements*)(arg);
    gui->changeRender(GuiElements::GuiGroup::CREATEMENU_GUI_GROUP);
}
void createServer(void* arg){
    GuiElements* gui = (GuiElements*)(arg);
    std::string passwordToServer = gui->getContainerList()[gui->getCurrentDisplayedGroup()]->getTextFieldList()[0]->getTextContainer();

    std::cout << "Password to server: " << passwordToServer << std::endl;
}

void startRendering(SDL_Window* window, SDL_Renderer* renderer, GuiElements* gui){
    // Render the first interface
    // SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    gui->changeRender(GuiElements::GuiGroup::LOGIN_GUI_GROUP);
    SDL_RenderPresent(renderer);
    
    SDL_Event event;
    bool running = true;
    while(running){
        SDL_WaitEvent(&event);
        switch (event.type){
            case SDL_QUIT:{
                running = false;
                break;
            }
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_TEXTINPUT:
            case SDL_KEYDOWN:{
                gui->handleEvent(event);

                SDL_RenderClear(renderer);
                gui->renderAll();
                SDL_RenderPresent(renderer);
                
                break;
            }
        }
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    delete gui;

    TTF_Quit();
    SDL_StopTextInput();
    SDL_Quit();
}

GuiElements* initInterface(SDL_Renderer* renderer, const int WINDOW_WIDTH, const int WINDOW_HEIGHT){
    GuiElements* gui = new GuiElements(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    if(TTF_Init() == -1){
        std::cout << TTF_GetError() << std::endl;
        return NULL;
    }
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG){
        std::cout << TTF_GetError() << std::endl;
        return NULL;
    }

    // Shared ressource between containers
    std::string fontPath = "Ressource/Zikketica.ttf";
    SDL_Color guiBgColor = {128, 128, 128};
    SDL_Color fontColor = {255, 255, 255};
    SDL_Rect rect = {gui->getCenterX() - 100, gui->getCenterY() - 100, 200, 35};

    // Login Container -------------------------------------------------
    Container* loginContainer = new Container(
        {gui->getCenterX() - 150, gui->getCenterY() - 75, 250, 245},
        true,
        {0, 153, 76, 255} , // old color: 51, 255, 153
        guiBgColor,
        fontPath,
        fontColor
    );
    
    TextField* usernameTF = loginContainer->addTextField(100, 35);
    TextField* passTF = loginContainer->addTextField(100, 35);
    Label* usernameL = loginContainer->addLabel("Username:", 35);
    Label* passwordL = loginContainer->addLabel("Password:", 35);
    TextButton* submitButton = loginContainer->addTextButton(100, 30, "Submit", &sendToMainMenu, gui);

    usernameTF->moveTo(
        loginContainer->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X),
        loginContainer->getAbsolutePositionY(Container::AbsolutePositionY::TOP_Y) + 50
    );
    usernameL->snapToLeftOf(usernameTF->getRect(), 10);
    passTF->snapToBottomOf(usernameTF->getRect(), 20);
    passwordL->snapToLeftOf(passTF->getRect(), 10);
    submitButton->moveTo(
        loginContainer->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X) - 50,
        loginContainer->getAbsolutePositionY(Container::AbsolutePositionY::BOTTOM_Y) - 70
    );

    // Main Menu Container -------------------------------------------------
    Container* mainMenuContainer = new Container(
        {gui->getCenterX() - 150, gui->getCenterY() - 75, 250, 245},
        true,
        {0, 153, 76, 255} , // old color: 51, 255, 153
        guiBgColor,
        fontPath,
        fontColor
    );

    TextButton* createGroupTB = mainMenuContainer->addTextButton(120, 35, "Create Group", &sendToCreateServerMenu, gui);
    TextButton* joinGroupTB = mainMenuContainer->addTextButton(120, 35, "Join Group", NULL, NULL);
    TextButton* logoutTB = mainMenuContainer->addTextButton(120, 35, "Logout", &sendToLoginMenu, gui);

    createGroupTB->moveTo(
        mainMenuContainer->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X) - 60,
        mainMenuContainer->getAbsolutePositionY(Container::AbsolutePositionY::TOP_Y) + 50
    );
    joinGroupTB->snapToBottomOf(createGroupTB->getRect(), 25);
    logoutTB->snapToBottomOf(joinGroupTB->getRect(), 25);

    // Create Menu Container -------------------------------------------------
    Container* createServerMenuContainer = new Container(
        {gui->getCenterX() - 150, gui->getCenterY() - 75, 250, 245},
        true,
        {0, 153, 76, 255} , // old color: 51, 255, 153
        guiBgColor,
        fontPath,
        fontColor
    );

    TextField* serverPasswordTF = createServerMenuContainer->addTextField(120, 35);
    Label* serverPasswordL = createServerMenuContainer->addLabel("Password: ", 35);
    TextButton* createServerTB = createServerMenuContainer->addTextButton(100, 35, "Create", &createServer, gui);
    TextButton* goBackServerTB = createServerMenuContainer->addTextButton(100, 35, "Go Back", &sendToMainMenu, gui);

    serverPasswordTF->moveTo(
        createServerMenuContainer->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X) - 10,
        createServerMenuContainer->getAbsolutePositionY(Container::AbsolutePositionY::TOP_Y) + 70
    );
    serverPasswordL->snapToLeftOf(serverPasswordTF->getRect(), 10);

    createServerTB->moveTo(
        createServerMenuContainer->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X) - 50,
        createServerMenuContainer->getAbsolutePositionY(Container::AbsolutePositionY::BOTTOM_Y) - 100
    );
    goBackServerTB->snapToBottomOf(createServerTB->getRect(), 5);


    gui->addContainer(loginContainer);
    gui->addContainer(mainMenuContainer);
    gui->addContainer(createServerMenuContainer);

    return gui;
}

void createWindow(){
    std::cout << "Main Called!" << std::endl;
    
    const int WINDOW_HEIGHT = 800;
    const int WINDOW_WIDTH = 1200;

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << SDL_GetError() << std::endl;
        return;
    }
    
    // Creation of the window
    SDL_Window* window = SDL_CreateWindow(
        "Interface test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if(!window){
        std::cout << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    // Initializing the gui elements and the renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_PRESENTVSYNC);
    if(!renderer){
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }
    
    GuiElements* gui = initInterface(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    if(!gui){
        std::cout << "initInterface returned NULL" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    startRendering(window, renderer, gui);
}

int main(int argc, char* argv[]){
    asio::io_context io;

    asio::ip::tcp::socket clientSocket(io);

    clientSocket.connect(
        asio::ip::tcp::endpoint(
            asio::ip::make_address("127.0.0.1"), 5544
        )
    );

    std::cout << "Interface.cpp : Connected to server" << std::endl;

    io.run();

    // try{
    //     asio::io_context io;

    //     ServerHandler server(io, 5544);

    //     io.run();
    // }
    // catch (std::exception& e){
    //     std::cout << e.what() << std::endl;
    // }

    // std::string* informationPacketTest = new std::string[1]{"Random information in the packet that will be displayed"};
    // Packet* test = new Packet(Packet::PacketType::MESSAGE, informationPacketTest);
    // test->printPacket();

    // std::string* loginInfo = new std::string[2]{"randomUsername123", "RandomPass321"};
    // Packet* loginTest = new Packet(Packet::PacketType::LOGIN, loginInfo);
    // loginTest->printPacket();

    // std::cout << "Before handle got : " << loginInfo[0].c_str() << " // " << loginInfo[1].c_str() << std::endl;
    // handlePacketDecoding(loginInfo, loginTest->getContainer());
    // std::cout << "After handle got : " << loginInfo[0].c_str() << " // " << loginInfo[1].c_str() << std::endl;


    // createWindow();
    return 0;
}