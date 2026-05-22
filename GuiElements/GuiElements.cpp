#include "GuiElements.h"

GuiElements::GuiElements(const int WINDOW_WIDTH, const int WINDOW_HEIGHT){
    this->WINDOW_HEIGHT = WINDOW_HEIGHT;
    this->WINDOW_WIDTH = WINDOW_WIDTH;
}
/** Delete also all the elements inside the gui lists */
GuiElements::~GuiElements(){
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


std::vector<TextField*> GuiElements::getTextFieldList() const{
    return this->guiTextFields;
}
void GuiElements::addTextField(TextField* textField){
    this->guiTextFields.push_back(textField);
}

std::vector<TextButton*> GuiElements::getTextButtonsList() const{
    return this->guiTextButtons;
}
void GuiElements::addTextButton(TextButton* textButton){
    this->guiTextButtons.push_back(textButton);
}

std::vector<Label*> GuiElements::getLabelsList() const{
    return this->guiLabels;
}
void GuiElements::addLabel(Label* label){
    this->guiLabels.push_back(label);
}


void GuiElements::renderAll(SDL_Renderer* renderer){
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