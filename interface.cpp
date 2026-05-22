#define SDL_MAIN_HANDLED
#include "GuiElements/GuiElements.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <iostream>

void func(void* arg){
    std::cout << "Im the button function!" << std::endl;
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

                    for(TextField* textField: gui->getTextFieldList()){
                        if(textField->isInside(mouseX, mouseY)){
                            std::cout << "TextField is selected" << std::endl;
                            textField->setIsSelected(true);
                        }else{
                            std::cout << "TextField is unselected" << std::endl;
                            textField->setIsSelected(false);
                        }
                    }
                    for(TextButton* textButton: gui->getTextButtonsList()){
                        if(textButton->isPressed(mouseX, mouseY)){
                            //TODO prolly nothing, isPressed already manage that
                        }
                    }
                    break;
                }
                case SDL_TEXTINPUT:{
                    for(TextField* textField: gui->getTextFieldList()){
                        if(textField->getIsSelected()){
                            // std::cout << "TextField Render about to add a char" << std::endl;
                            std::cout << "TextField about to add the char " << event.text.text << std::endl;
                            textField->addToTextContainer(event.text.text);
                        }
                    }
                    break;
                }
                case SDL_KEYDOWN:{
                    for(TextField* textField: gui->getTextFieldList()){
                        if(textField->getIsSelected()){
                            if(event.key.keysym.sym == SDLK_BACKSPACE){
                                std::cout << "TextField about to remove a char" << std::endl;
                                textField->removeFromTextContainer();
                            }
                        }
                    }
                    break;
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderClear(renderer);
        
        // SDL_Rect rect = {50, 50, 50, 50};
        // SDL_SetRenderDrawColor(renderer, 0, 120, 255, 255);
        // SDL_RenderFillRect(renderer, &rect);
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

    // X Position, Y Position, Width, Height
    SDL_Rect tf1Rect = {gui->getCenterX(), 3 * gui->getCenterY() / 2, 50, 30};
    TextField* tf1 = new TextField(tf1Rect, bgColor, fontPath, fontColor);
    gui->addTextField(tf1);
    
    SDL_Rect tb1Rect = {gui->getCenterX(), gui->getCenterY(), 200, 30};
    TextButton* tbutton1 = new TextButton(tb1Rect, bgColor, "Press Me!", fontPath, fontColor, &func, NULL);
    gui->addTextButton(tbutton1);

    // On purpose setting the x position to 2000 to try the snapToLeft
    SDL_Rect lb1Rect = {2000, gui->getCenterY(), 50, 30};
    Label* label1 = new Label(lb1Rect, "Test Label: ", fontPath, fontColor);
    gui->addLabel(label1);
    
    label1->snapToTopOf(tf1->getRect(), 0);
    tbutton1->snapToBottomOf(tf1->getRect(), 10);
    

    //Init of the interface
    // SDL_Rect rect = {250, 150, 300, 200};
    // SDL_SetRenderDrawColor(renderer, 0, 120, 255, 255);
    // SDL_RenderFillRect(renderer, &rect);

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