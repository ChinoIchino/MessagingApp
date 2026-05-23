#define SDL_MAIN_HANDLED
#include "GuiElements/GuiElements.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <iostream>

void collectTextFieldInformation(void* arg){
    // GuiElements* gui = (GuiElements*)(arg);
    
    // std::string username = gui->getTextFieldList()[0]->getTextContainer();

    // std::cout 
    //     << "collectTextFieldInformation: Username: " << gui->getTextFieldList()[0]->getTextContainer() 
    //     << " // Password: " << gui->getTextFieldList()[1]->getTextContainer() 
    // << std::endl;
}

void startRendering(SDL_Window* window, SDL_Renderer* renderer, GuiElements* gui){
    SDL_Event event;
    
    bool running = true;
    while(running){
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:{
                    running = false;
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:{
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    for(Container* container: gui->getContainerList()){
                        container->handleMouseInput(mouseX, mouseY);
                    }
                    break;
                }
                case SDL_TEXTINPUT:{
                    for(Container* container: gui->getContainerList()){
                        for(TextField* textField: container->getTextFieldList()){
                            if(textField->getIsSelected()){
                                // std::cout << "TextField Render about to add a char" << std::endl;
                                std::cout << "TextField about to add the char " << event.text.text << std::endl;
                                textField->addToTextContainer(event.text.text);
                            }
                        }
                    }
                    break;
                }
                case SDL_KEYDOWN:{
                    for(Container *container: gui->getContainerList()){
                        for(TextField* textField: container->getTextFieldList()){
                            if(textField->getIsSelected()){
                                if(event.key.keysym.sym == SDLK_BACKSPACE){
                                    std::cout << "TextField about to remove a char" << std::endl;
                                    textField->removeFromTextContainer();
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderClear(renderer);
        
        gui->renderAll(renderer);
        
        SDL_RenderPresent(renderer);
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

    SDL_Rect c1Rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    Container* c1 = new Container(c1Rect, {128, 128, 128});
    c1->setHandleInput(false);

    TextField* nameTextField = new TextField(rect, bgColor, fontPath, fontColor);
    TextField* passTextField = new TextField(rect, bgColor, fontPath, fontColor);
    c1->addTextField(nameTextField);
    c1->addTextField(passTextField);

    Label* nameLabel = new Label(rect, "Username: ", fontPath, fontColor);
    Label* passLabel = new Label(rect, "Password: ", fontPath, fontColor);
    c1->addLabel(nameLabel);
    c1->addLabel(passLabel);

    TextButton* submitButton = new TextButton(rect, bgColor, "Submit", fontPath, fontColor, &collectTextFieldInformation, (void*)(gui));
    c1->addTextButton(submitButton);

    nameLabel->snapToLeftOf(nameTextField->getRect(), 10);
    passTextField->snapToBottomOf(nameTextField->getRect(), 10);
    passLabel->snapToBottomOf(nameLabel->getRect(), 10);
    submitButton->snapToBottomOf(passTextField->getRect(), 20);

    // Container
    SDL_Rect containerRect = {gui->getCenterX(), gui->getCenterY(), 200, 200};
    SDL_Color containerColor = {128, 0, 255};

    Container* c2 = new Container(containerRect, containerColor);

    gui->addContainer(c1);
    gui->addContainer(c2);

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
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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