#include "GuiElements.h"
#include <iostream>

GuiElements::GuiElements(SDL_Renderer* renderer, const int WINDOW_WIDTH, const int WINDOW_HEIGHT){
    this->rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    this->backgroundColor = {128, 128, 128};

    this->renderer = renderer;

    this->WINDOW_HEIGHT = WINDOW_HEIGHT;
    this->WINDOW_WIDTH = WINDOW_WIDTH;

    this->leftSideOutline = IMG_LoadTexture(renderer, "Ressource/Img/BorderLeft.png");
    this->rightSideOutline = IMG_LoadTexture(renderer, "Ressource/Img/BorderRight.png");
    this->middleOutline = IMG_LoadTexture(renderer, "Ressource/Img/BorderMiddle.png");
}
/** 
 * Delete all the containers (and elements inside those containers too)
 * It does not delete the SDL_Renderer
 */
GuiElements::~GuiElements(){
    for(Container* container: this->guiContainers){
        delete container;
    }
    this->guiContainers.clear();
}

void GuiElements::changeRender(GuiGroup indexToRender){
    if(!this->renderer){
        return;
    }

    std::cout << "About to change the render " << std::endl;
    SDL_RenderClear(this->renderer);
    
    std::vector<Container*> containerList = this->getContainerList();
    for(int i = 0; i < containerList.size(); i++){
        if(i == indexToRender){
            containerList[i]->setHandleInput(true);
            containerList[i]->setIsVisible(true);
            containerList[i]->render(this->renderer);
        }else{
            containerList[i]->setHandleInput(false);
            containerList[i]->setIsVisible(false);
        }
    }
    this->currentDisplayedGroup = indexToRender;
}

void GuiElements::handleEvent(SDL_Event event){
    switch(event.type){
        case SDL_MOUSEBUTTONDOWN:{
            this->guiContainers[this->currentDisplayedGroup]->handleMouseInput(event.button.x, event.button.y);
            break;
        }
        case SDL_TEXTINPUT:{
            this->guiContainers[this->currentDisplayedGroup]->handleTextInput(event.text.text, this->renderer);
            break;
        }
        case SDL_KEYDOWN:{
            this->guiContainers[this->currentDisplayedGroup]->handleKeyInput(event.key.keysym.sym, this->renderer);
            break;
        }
        case SDL_MOUSEMOTION:{
            this->guiContainers[this->currentDisplayedGroup]->handleMouseMotion(event.button.x, event.button.y);
            break;
        }
        default:
            std::cout << "GuiElements::handleEvent, this event type is not handled"  << std::endl;
            break;
        };
}

int GuiElements::getCurrentDisplayedGroup() const{
    return this->currentDisplayedGroup;
}

std::vector<Container*> GuiElements::getContainerList() const{
    return this->guiContainers;
}
void GuiElements::addContainer(Container* container){
    container->loadOutline(this->leftSideOutline, this->rightSideOutline, this->middleOutline);
    container->loadAllTextures(this->renderer);
    this->guiContainers.push_back(container);
}


void GuiElements::renderAll(){
    SDL_SetRenderDrawColor(renderer, this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a);
    SDL_RenderFillRect(renderer, &this->rect);
    
    for(Container* container: this->guiContainers){
        container->render(this->renderer);
    }
}