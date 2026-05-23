#include "Container.h"

Container::Container(SDL_Rect rect, SDL_Color backgroundColor){
    this->rect = rect;
    this->backgroundColor = backgroundColor;

    this->visible = true;
    this->handleInput = true;
}

Container::~Container(){
    for(TextField* textField: this->guiTextFields){
        delete textField;
    }
    this->guiTextFields.clear();

    for(TextButton* textButton: this->guiTextButtons){
        delete textButton;
    }
    this->guiTextButtons.clear();

    for(Label* label: this->guiLabels){
        delete label;
    }
    this->guiLabels.clear();
}

void Container::render(SDL_Renderer* renderer){
    if(!this->visible){
        return;
    }
    
    SDL_SetRenderDrawColor(renderer, this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a);
    SDL_RenderFillRect(renderer, &this->rect);
    
    for(TextField* textField: this->guiTextFields){
        textField->render(renderer);
    }
    for(TextButton* textButton: this->guiTextButtons){
        textButton->render(renderer);
    }
    for(Label* label: this->guiLabels){
        label->render(renderer);
    }
}

/** 
 * Handle every isPressed/isInside function of every interactable gui elements
 * inside the container
 * All interacted item will have their states changed based on the input 
 * (ex: buttons function will be activated, textfield will be selected)
 * @return true if at least one item was interacted with. Else return false
 */
bool Container::handleMouseInput(int x, int y){
    if(!this->handleInput){
        return false;
    }

    bool result = false;
    for(TextField* textField: this->guiTextFields){
        if(textField->isInside(x, y)){
           result = true;
           textField->setIsSelected(true);
        }else{
            textField->setIsSelected(false);
        }
    }
    for(TextButton* textButton: this->guiTextButtons){
        if(textButton->isPressed(x,y)){
            result = true;
        }
    }

    return result;
}

std::vector<TextField*> Container::getTextFieldList() const{
    return this->guiTextFields;
}
void Container::addTextField(TextField* textField){
    this->guiTextFields.push_back(textField);
}

std::vector<TextButton*> Container::getTextButtonsList() const{
    return this->guiTextButtons;
}
void Container::addTextButton(TextButton* textButton){
    this->guiTextButtons.push_back(textButton);
}

std::vector<Label*> Container::getLabelsList() const{
    return this->guiLabels;
}
void Container::addLabel(Label* label){
    this->guiLabels.push_back(label);
}

int Container::getCenterX(){
    return this->rect.x;
}
int Container::getCenterY(){
    return this->rect.y;
}

int Container::getHeight(){
    return this->rect.h;
}
int Container::getWidth(){
    return this->rect.w;
}


void Container::setIsVisible(bool isVisible){
    this->visible = isVisible;
}
bool Container::isVisible() const{
    return this->visible;
}

void Container::setHandleInput(bool isHandling){
    this->handleInput = isHandling;
}
bool Container::isHandlingInput() const{
    return this->handleInput;
}