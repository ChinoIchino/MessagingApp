#define SDL_MAIN_HANDLED

#include "GuiElements/GuiElements.h"
#include "ClientSession.h"
// #include "Packet.h"

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

void createFakePacket(void* arg){
    auto clientSession = (std::shared_ptr<ClientSession>*) arg;

    Packet* packetToSend = new Packet(Packet::PacketType::MESSAGE, new std::string("Random message packet that was sent via the createFakePacket function !"));

    clientSession->get()->write(packetToSend);
    // clientSession->write(packetToSend); 
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

GuiElements* initInterface(SDL_Renderer* renderer, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, std::shared_ptr<ClientSession>* clientSession){
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
        WINDOW_HEIGHT,
        WINDOW_WIDTH,
        //{gui->getCenterX() - 150, gui->getCenterY() - 75, 250, 245},
        true,
        {0, 153, 76, 255} , // old color: 51, 255, 153
        guiBgColor,
        fontPath,
        fontColor
    );
    loginContainer->setRectSize(0.5, 0.5);
    
    TextField* usernameTF = loginContainer->addTextField()->snapToTop(200);
    TextField* passTF = loginContainer->addTextField()->snapToTop(20);
    Label* usernameL = loginContainer->addLabel("Username:")->centerGroup(usernameTF, 10);
    Label* passwordL = loginContainer->addLabel("Password:")->centerGroup(passTF, 10);
    TextButton* submitButton = loginContainer->addTextButton("Submit", &sendToMainMenu, gui)->snapToTop(30);

    // Main Menu Container -------------------------------------------------
    Container* mainMenuContainer = new Container(
        WINDOW_HEIGHT,
        WINDOW_WIDTH,
        //{gui->getCenterX() - 150, gui->getCenterY() - 75, 250, 245},
        true,
        {0, 153, 76, 255} , // old color: 51, 255, 153
        guiBgColor,
        fontPath,
        fontColor
    );
    mainMenuContainer->setRectSize(0.5, 0.5);

    TextButton* createGroupTB = mainMenuContainer->addTextButton("Create Group", &sendToCreateServerMenu, gui)->snapToTop(200);
    TextButton* joinGroupTB = mainMenuContainer->addTextButton("Join Group", NULL, NULL)->snapToTop(20);
    TextButton* logoutTB = mainMenuContainer->addTextButton("Logout", &sendToLoginMenu, gui)->snapToTop(20);

    // Create Menu Container -------------------------------------------------
    Container* createServerMenuContainer = new Container(
        WINDOW_HEIGHT,
        WINDOW_WIDTH,
        //{gui->getCenterX() - 150, gui->getCenterY() - 75, 250, 245},
        true,
        {0, 153, 76, 255} , // old color: 51, 255, 153
        guiBgColor,
        fontPath,
        fontColor
    );
    createServerMenuContainer->setRectSize(0.5, 0.5);

    TextField* serverPasswordTF = createServerMenuContainer->addTextField()->snapToTop(200);
    Label* serverPasswordL = createServerMenuContainer->addLabel("Password: ")->centerGroup(serverPasswordTF, 10);
    TextButton* createServerTB = createServerMenuContainer->addTextButton("Create", &createFakePacket, clientSession)->snapToTop(20);
    TextButton* goBackServerTB = createServerMenuContainer->addTextButton("Go Back", &sendToMainMenu, gui)->snapToTop(30);

    gui->addContainer(loginContainer);
    gui->addContainer(mainMenuContainer);
    gui->addContainer(createServerMenuContainer);

    return gui;
}

void createWindow(std::shared_ptr<ClientSession>* clientSession){
    std::cout << "Main Called!" << std::endl;
    
    const int WINDOW_HEIGHT = 1600;
    const int WINDOW_WIDTH = 2400;

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
    
    GuiElements* gui = initInterface(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, clientSession);

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

    auto clientSession = std::make_shared<ClientSession>(std::move(clientSocket));

    std::cout << "Interface.cpp : Connected to server" << std::endl;

    io.run();

    createWindow(&clientSession);
    return 0;
}