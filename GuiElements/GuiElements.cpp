#include "GuiElements.h"
#include <iostream>

GuiElements::GuiElements(const int WINDOW_WIDTH, const int WINDOW_HEIGHT){
    this->WINDOW_HEIGHT = WINDOW_HEIGHT;
    this->WINDOW_WIDTH = WINDOW_WIDTH;
}
/** Delete also all the elements inside the gui lists */
GuiElements::~GuiElements(){
    for(Container* container: this->guiContainers){
        delete container;
    }
    this->guiContainers.clear();
}

void GuiElements::changeRender(GuiGroup indexToRender, SDL_Renderer* renderer){
    std::cout << "About to change the render " << std::endl;
    SDL_RenderClear(renderer);
    
    std::vector<Container*> containerList = this->getContainerList();
    for(int i = 0; i < containerList.size(); i++){
        if(i == indexToRender){
            containerList[i]->setHandleInput(true);
            containerList[i]->setIsVisible(true);
            containerList[i]->render(renderer);
        }else{
            containerList[i]->setHandleInput(false);
            containerList[i]->setIsVisible(false);
        }
    }
    this->currentDisplayedGroup = indexToRender;
}

void GuiElements::handleEvent(SDL_Event event, SDL_Renderer* renderer){
    switch(event.type){
        case SDL_MOUSEBUTTONDOWN:{
            this->guiContainers[this->currentDisplayedGroup]->handleMouseInput(event.button.x, event.button.y);
            break;
        }
        case SDL_TEXTINPUT:{
            this->guiContainers[this->currentDisplayedGroup]->handleTextInput(event.text.text, renderer);
            break;
        }
        case SDL_KEYDOWN:{
            this->guiContainers[this->currentDisplayedGroup]->handleKeyInput(event.key.keysym.sym, renderer);
            break;
        }
    
    default:
        break;
    };
}

std::vector<Container*> GuiElements::getContainerList() const{
    return this->guiContainers;
}
void GuiElements::addContainer(Container* container){
    this->guiContainers.push_back(container);
}


void GuiElements::renderAll(SDL_Renderer* renderer){
    for(Container* container: this->guiContainers){
        container->render(renderer);
    }
}