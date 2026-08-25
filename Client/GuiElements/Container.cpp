#include "Container.h"
#include <iostream>

Container::Container(int windowHeight, int windowWidth, bool isRounded, SDL_Color backgroundColor, SDL_Color guiElementsBackgroundColor, std::string filePathToFont, SDL_Color fontColor)
    : WINDOW_HEIGHT(windowHeight), WINDOW_WIDTH(windowWidth){

    this->chosenLayout = GuiLayout::DEFAULT;

    this->ammountOfItems = 0;
    this->availableTopPosition = 0;
    
    this->rect = {0, 0, WINDOW_HEIGHT, WINDOW_WIDTH};
    this->rounded = isRounded;

    this->backgroundColor = backgroundColor;
    this->guiBackgroundColor = guiElementsBackgroundColor;

    this->fontPath = filePathToFont;
    this->fontColor = fontColor;

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

void Container::loadOutline(SDL_Texture* leftSideOutline, SDL_Texture* rightSideOutline, SDL_Texture* middleOutline){
    this->leftSideOutline = leftSideOutline;
    this->rightSideOutline = rightSideOutline;
    this->middleOutline = middleOutline;
}
void Container::loadAllTextures(SDL_Renderer* renderer){
    for(TextField* textField: this->guiTextFields){
        textField->loadTextures();
    }
    for(TextButton* textButton: this->guiTextButtons){
        textButton->loadTexture(renderer);
    }
}

void Container::render(SDL_Renderer* renderer){
    if(!this->visible){
        return;
    }
    if(this->rounded){
        roundedBoxRGBA(
            renderer,
            this->rect.x, this->rect.y,
            this->rect.x + this->rect.w, this->rect.y + this->rect.h,
            8,
            this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a
        );
    }else{
        SDL_SetRenderDrawColor(renderer, this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a);
        SDL_RenderFillRect(renderer, &this->rect);
    }
    
    for(TextField* textField: this->guiTextFields){
        textField->render(renderer);
        textField->renderOutline(renderer, this->leftSideOutline, this->rightSideOutline, this->middleOutline);
    }
    for(TextButton* textButton: this->guiTextButtons){
        textButton->render(renderer);
        textButton->renderOutline(renderer, this->leftSideOutline, this->rightSideOutline, this->middleOutline);
    }
    for(Label* label: this->guiLabels){
        label->render(renderer);
    }
    for(ScrollPane* scrollPane: this->guiScrollPanes){
        scrollPane->render(renderer);
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
bool Container::handleMouseScroll(int verticalScrollAmm){
    for(ScrollPane* scrollPane: this->guiScrollPanes){
        if(scrollPane->isHovering()){
            scrollPane->modifyScrollOffset(verticalScrollAmm);
            return true;
        }
    }
    return false;
}
bool Container::handleMouseMotion(int x, int y){
    bool result = false;

    for(TextButton* textButton: this->guiTextButtons){
        if(textButton->isInside(x, y)){
            textButton->setIsHovering(true);
            result = true;
        }else{
            textButton->setIsHovering(false);
        }
    }
    for(ScrollPane* scrollPane: this->guiScrollPanes){
        if(scrollPane->isInside(x, y)){
            scrollPane->setIsHovering(true);
            result = true;
        }else{
            scrollPane->setIsHovering(false);
        }
    }
    return result;
}
bool Container::handleTextInput(std::string input, SDL_Renderer* renderer){
    for(TextField* textField: this->guiTextFields){
        if(textField->isSelected()){
            textField->addToTextContainer(input);
            textField->updateTextState(renderer);
            return true;
        }
    }
    return false;
}
bool Container::handleKeyInput(SDL_Keycode key, SDL_Renderer* renderer){
    for(TextField* textField: this->guiTextFields){
        if(textField->isSelected()){
            if(key == SDLK_BACKSPACE){
                textField->removeFromTextContainer();
                textField->updateTextState(renderer);
                return true;
            }
        }
    }
    return false;
}


std::vector<TextField*> Container::getTextFieldList() const{
    return this->guiTextFields;
}
TextField* Container::addTextField(int width, int height){
    TextField* toAdd = new TextField(
        this->ammountOfItems++,
        this,
        {0, 0, width, height},
        this->guiBackgroundColor,
        this->fontPath.c_str(),
        fontColor
    );
    this->guiTextFields.push_back(toAdd);

    if(this->chosenLayout != GuiLayout::DEFAULT){
        this->reformatGui();
    }

    return toAdd;
}
TextField* Container::addTextField(){
    TextField* toAdd = new TextField(
        this->ammountOfItems++,
        this,
        {0, 0, this->rect.w / 4, this->rect.h / 10},
        this->guiBackgroundColor,
        this->fontPath.c_str(),
        fontColor
    );
    this->guiTextFields.push_back(toAdd);

    if(this->chosenLayout != GuiLayout::DEFAULT){
        this->reformatGui();
    }


    return toAdd;
}

std::vector<TextButton*> Container::getTextButtonsList() const{
    return this->guiTextButtons;
}
TextButton* Container::addTextButton(int width, int height, std::string displayText, void (*reactFunction)(void* arg), void* arg){
    TextButton* toAdd = new TextButton(
        this->ammountOfItems++,
        this,
        {0, 0, width, height},
        this->guiBackgroundColor,
        displayText,
        this->fontPath.c_str(),
        this->fontColor,
        reactFunction,
        arg
    );
    this->guiTextButtons.push_back(toAdd);

    if(this->chosenLayout != GuiLayout::DEFAULT){
        this->reformatGui();
    }

    return toAdd;
}
TextButton* Container::addTextButton(std::string displayText, void (*reactFunction)(void* arg), void* arg){
    TextButton* toAdd = new TextButton(
        this->ammountOfItems++,
        this,
        {0, 0, this->rect.w / 3, this->rect.h / 10},
        this->guiBackgroundColor,
        displayText,
        this->fontPath.c_str(),
        this->fontColor,
        reactFunction,
        arg
    );
    this->guiTextButtons.push_back(toAdd);

    if(this->chosenLayout != GuiLayout::DEFAULT){
        this->reformatGui();
    }

    return toAdd;
}

std::vector<Label*> Container::getLabelsList() const{
    return this->guiLabels;
}
Label* Container::addLabel(std::string displayedText, int fontSize){
    Label* toAdd = new Label(
        this->ammountOfItems++,
        this,
        {0, 0, 0, fontSize},
        displayedText,
        this->fontPath.c_str(),
        this->fontColor    
    );
    this->guiLabels.push_back(toAdd);

    if(this->chosenLayout != GuiLayout::DEFAULT){
        this->reformatGui();
    }

    return toAdd;
}
Label* Container::addLabel(std::string displayedText){
    Label* toAdd = new Label(
        this->ammountOfItems++,
        this,
        {0, 0, 0, this->rect.h / 10},
        displayedText,
        this->fontPath.c_str(),
        this->fontColor
    );
    this->guiLabels.push_back(toAdd);

    if(this->chosenLayout != GuiLayout::DEFAULT){
        this->reformatGui();
    }

    return toAdd;
}

ScrollPane* Container::addScrollPane(){
    ScrollPane* toAdd = new ScrollPane(
        this->rect.x,
        this->rect.y,
        this->rect.w,
        this->rect.h,
        this->fontPath
    );
    this->guiScrollPanes.push_back(toAdd);

    return toAdd;
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

int Container::getAbsolutePositionX(AbsolutePositionX absX){
    if(absX == LEFT_X){
        return this->rect.x;
    }
    if(absX == MIDDLE_X){
        return this->rect.x + (this->rect.w / 2);
    }
    if(absX == RIGHT_X){
        return this->rect.x + rect.w;
    }
    return 0;
}
int Container::getAbsolutePositionY(AbsolutePositionY absY){
    if(absY == TOP_Y){
        return this->rect.y;
    }
    if(absY == MIDDLE_Y){
        return this->rect.y + (this->rect.h / 2);
    }
    if(absY == BOTTOM_Y){
        return this->rect.y + rect.h;
    }
    return 0;
}

int Container::claimTopPosition(int itemHeight, int padding){
    int availablePos = this->availableTopPosition + padding;
    
    this->availableTopPosition += itemHeight + padding;
    return availablePos;
}
int Container::claimTopPosition(int itemHeight){
    int availablePos = this->availableTopPosition;
    
    this->availableTopPosition += itemHeight;
    return availablePos;
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

/**
 * NEED TO BE USED BEFORE GUI ELEMENTS ARE ADDED INTO THE CONTAINER.
 * Change the size based on the application window size.
 * @param height : Percentage of the application window to be used vertically
 * @param height : Percentage of the application window to be used horizontally
 */
void Container::setRectSize(float height, float width){
    this->rect = {(int)((1.0f - width) / 2 * this->WINDOW_WIDTH), (int)((1.0f - height) / 2 * this->WINDOW_HEIGHT), (int)(this->WINDOW_WIDTH * width), (int)(this->WINDOW_HEIGHT * height)};

    this->availableTopPosition = this->rect.y;
}

void Container::setGuiLayout(GuiLayout newLayout){
    this->chosenLayout = newLayout;

    //TODO reform the scene based on the new layout
}

UiElement* Container::searchById(int id){
    for(TextField* textField: this->guiTextFields){
        if(id == textField->getId()){
            return textField;
        }
    }
    for(TextButton* textButton: this->guiTextButtons){
        if(id == textButton->getId()){
            return textButton;
        }
    }
    for(Label* label: this->guiLabels){
        if(id == label->getId()){
            return label;
        }
    }

    return NULL;
}

void Container::reformatGui(){
    UiElement* currItem;
    int gridSize = this->rect.h / this->ammountOfItems;
    int offsetY = this->rect.y;
    
    for(int currId = 0; currId < this->ammountOfItems; currId++){
        currItem = searchById(currId);
        if(currItem == NULL){
            std::cout << "Didn't found anything under the id : " << currId << std::endl;
            break;
        }
        currItem->moveTo(
            this->getAbsolutePositionX(AbsolutePositionX::MIDDLE_X) - currItem->getRect().w  / 2,
            gridSize / 2 + offsetY - currItem->getRect().h / 2
        );

        offsetY += gridSize;
    }
}