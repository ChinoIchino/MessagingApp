#define SDL_MAIN_HANDLED
#include "GuiElements/GuiElements.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <iostream>

// TODO make a function that connect to a database of users
void collectTextFieldInformation(void* arg){
    // GuiElements* gui = (GuiElements*)(arg);
    
    // std::string username = gui->getTextFieldList()[0]->getTextContainer();

    // std::cout 
    //     << "collectTextFieldInformation: Username: " << gui->getTextFieldList()[0]->getTextContainer() 
    //     << " // Password: " << gui->getTextFieldList()[1]->getTextContainer() 
    // << std::endl;
}

void startRendering(SDL_Window* window, SDL_Renderer* renderer, GuiElements* gui){
    // Render the first interface
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    gui->changeRender(GuiElements::GuiGroup::LOGIN_GUI_GROUP, renderer);
    SDL_RenderPresent(renderer);
    
    SDL_Event event;
    bool running = true;
    while(running){
        while(SDL_WaitEvent(&event)){
            switch (event.type){
                case SDL_QUIT:{
                    running = false;
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                case SDL_TEXTINPUT:
                case SDL_KEYDOWN:{
                    gui->handleEvent(event, renderer);
                    SDL_RenderPresent(renderer);
                    break;
                }
            }
        }
        
        // SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        // SDL_RenderClear(renderer);
        
        // gui->renderAll(renderer);
        
        // SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    delete gui;

    TTF_Quit();
    SDL_StopTextInput();
    SDL_Quit();
}

GuiElements* initInterface(const int WINDOW_WIDTH, const int WINDOW_HEIGHT){
    GuiElements* gui = new GuiElements(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    if (TTF_Init() == -1){
        std::cout << TTF_GetError() << std::endl;
        return NULL;
    }

    // TTF_Font* font = TTF_OpenFont("Ressource/Zikketica.ttf", 14);
    const char* fontPath = "Ressource/Zikketica.ttf";
    SDL_Color bgColor = {0, 0, 0};
    SDL_Color fontColor = {255, 255, 255};
    SDL_Rect rect = {gui->getCenterX() - 100, gui->getCenterY() - 100, 200, 35};

    // Login Container
    SDL_Rect container2Rect = {gui->getCenterX() - 150, gui->getCenterY() - 75, 300, 250};
    SDL_Color container2Color = {51, 255, 153};

    Container* loginContainer = new Container(container2Rect, container2Color);

    TextField* usernameTF = new TextField(
        {
            loginContainer->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X),
            loginContainer->getAbsolutePositionY(Container::AbsolutePositionY::TOP_Y) + 50, 
            100, 
            35
        },
        bgColor,
        fontPath,
        fontColor
    );
    
    TextField* passTF = new TextField(
        {0, 0, 100, 35},
        bgColor,
        fontPath,
        fontColor
    );
    passTF->snapToBottomOf(usernameTF->getRect(), 50);

    Label* usernameL = new Label(
        {0, 0, 0, 35},
        "Username",
        fontPath,
        fontColor
    );
    usernameL->snapToLeftOf(usernameTF->getRect(), 10);

    Label* passL = new Label(
        {0, 0, 0, 35},
        "Password",
        fontPath,
        fontColor
    );
    passL->snapToLeftOf(passTF->getRect(), 10);

    TextButton* submitTB = new TextButton(
        {
            loginContainer->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X) - 50,
            loginContainer->getAbsolutePositionY(Container::AbsolutePositionY::BOTTOM_Y) - 50, 
            100, 
            35
        },
        bgColor,
        "Submit",
        fontPath,
        fontColor,
        NULL,
        NULL
    );

    loginContainer->addTextField(usernameTF);
    loginContainer->addTextField(passTF);
    loginContainer->addLabel(usernameL);
    loginContainer->addLabel(passL);
    loginContainer->addTextButton(submitTB);

    gui->addContainer(loginContainer);

    return gui;
}

void CreateWindow(){
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
    
    GuiElements* gui = initInterface(WINDOW_WIDTH, WINDOW_HEIGHT);

    if(!gui){
        std::cout << "initInterface returned NULL" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    startRendering(window, renderer, gui);
}


int main(int argc, char* argv[]){
    CreateWindow();
    return 0;
}