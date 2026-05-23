#include "GuiElements.h"
#include <iostream>

enum Screen{
    LOGIN_SCREEN,
    MAIN_SCREEN
};

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