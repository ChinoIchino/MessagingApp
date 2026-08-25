// #include "TextField.h"
#include "Container.h"
#include <iostream>

TextField::TextField(int id, Container* parent, SDL_Rect rect, SDL_Color bgColor, const char* filePathToFont, SDL_Color fontColor){
    this->parent = parent;

    this->id = id;
    
    this->rect = rect;
    this->backgroundColor = bgColor;
    
    this->font = TTF_OpenFont(filePathToFont, 50);

    if(!this->font){
        std::cout << "TextField font failed: "<< TTF_GetError() << std::endl;
    }

    this->fontColor = fontColor;

    this->surface = nullptr;
    this->texture = nullptr;

    this->visible = true;
}

TextField::~TextField(){
    if(this->surface != nullptr){
        SDL_FreeSurface(this->surface);
    }
    if(this->texture != nullptr){
        SDL_DestroyTexture(this->texture);
    }
}

void TextField::loadTextures(){
    // Give them a position and size (7 because the textures are 7 pixels wide)
    this->leftTexRect = {this->rect.x, this->rect.y, 7, this->rect.h};
    this->rightTexRect = {this->rect.x + this->rect.w - 7 , this->rect.y, 7, this->rect.h};
    this->middleTexRect = {this->rect.x + 7, this->rect.y, this->rect.w - 14, this->rect.h};
}

bool TextField::isSelected() const{
    return this->selected;
}
void TextField::setIsSelected(bool newStatus){
    this->selected = newStatus;
}

bool TextField::isVisible() const{
    return this->visible;
}
void TextField::setIsVisible(bool isVisible){
    this->visible = isVisible;
}

std::string TextField::getTextContainer() const{
    return this->textContainer;
}
SDL_Rect TextField::getRect() const{
    return this->rect;
}
int TextField::getId() const{
    return this->id;
}

TextField* TextField::snapToTop(int padding){
    int width = this->rect.w;
    int height = this->rect.h;

    this->rect = {
        this->parent->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X) - width / 2,
        this->parent->claimTopPosition(height, padding),
        width,
        height
    };

    return this;
}
TextField* TextField::snapToTop(){
    int width = this->rect.w;
    int height = this->rect.h;

    this->rect = {
        this->parent->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X) - width / 2,
        this->parent->claimTopPosition(height),
        width,
        height
    };

    return this;
}

void TextField::moveTo(int x, int y){
    int width = this->rect.w;
    int height = this->rect.h;

    this->rect = {x, y, width, height};
}
void TextField::snapToLeftOf(SDL_Rect rectToSnap, int padding){
    int height = this->rect.h;
    int width = this->rect.w;

    this->rect = {
        rectToSnap.x - width - padding,
        rectToSnap.y,
        width,
        height
    };
}
void TextField::snapToRightOf(SDL_Rect rectToSnap, int padding){
    int height = this->rect.h;
    int width = this->rect.w;

    this->rect = {
        rectToSnap.x + rectToSnap.w + padding,
        rectToSnap.y,
        width,
        height
    };
}
void TextField::snapToBottomOf(SDL_Rect rectToSnap, int padding){
    int height = this->rect.h;
    int width = this->rect.w;

    this->rect = {
        rectToSnap.x,
        rectToSnap.y + height + padding,
        width,
        height
    };
}
void TextField::snapToTopOf(SDL_Rect rectToSnap, int padding){
    int height = this->rect.h;
    int width = this->rect.w;

    this->rect = {
        rectToSnap.x,
        rectToSnap.y - height - padding,
        width,
        height
    };
}

void TextField::render(SDL_Renderer* renderer){
    if(!this->visible){
        return;
    }

    SDL_SetRenderDrawColor(renderer, this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a);
    SDL_RenderFillRect(renderer, &this->rect);

    // If there is no text to be processed, there is no need for the function to continue
    if(this->textContainer.empty()){
        return;
    }

    if(this->texture == NULL){
        std::cout << "TextField::render: this->texture is NULL, Stopping the render" << std::endl;
        return;
    }

    SDL_RenderCopy(renderer, this->texture, NULL, &this->textRect);
}
void TextField::renderOutline(SDL_Renderer* renderer, SDL_Texture* texLeftSide, SDL_Texture* texRigthSide, SDL_Texture* texMiddleSide){
    // Load all the textures used. 3 textures are used to avoid strange stretches of textures
    if(texLeftSide == NULL || texRigthSide == NULL || texMiddleSide == NULL){
        std::cout << "Error TextButton::render : One or more textures didn't loaded, stopping the render" << std::endl;
        return;
    }
    
    // Render the 3 textures on top of the TextButton rectangle
    SDL_RenderCopy(renderer, texMiddleSide, NULL, &this->middleTexRect);
    SDL_RenderCopy(renderer, texLeftSide, NULL, &this->leftTexRect);
    SDL_RenderCopy(renderer, texRigthSide, NULL, &this->rightTexRect);
}

void TextField::updateTextState(SDL_Renderer* renderer){
    if(this->texture != nullptr){
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }

    // Get only a part of the text based on the text field size
    std::string visibleText = textContainer;
    int currTextWidth = 0;
    // Add a offset so doesn't matter what is the char width it doesn't move
    bool ignoreOffset = true;
    int offset = 0;
    while(!visibleText.empty()){
        TTF_SizeText(
            this->font,
            this->textContainer.c_str(),
            &currTextWidth,
            NULL
        );

        if(currTextWidth <= this->rect.w - 10){
            offset = this->rect.w - currTextWidth - 10;
            break;
        }
        ignoreOffset = false;
        visibleText.erase(0, 1);
    }
    if(ignoreOffset){
        offset = 0;
    }

    // Display the last x char based on their size
    this->surface = TTF_RenderText_Solid(this->font, visibleText.c_str(), this->fontColor);
    this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);

    SDL_FreeSurface(this->surface);

    if(this->texture == NULL){
        std::cout << "TextField::updateTextState: this->texture is NULL, Stopping the update" << std::endl;
        return;
    }

    this->textRect = {
        this->rect.x + offset + 5,
        this->rect.y + surface->h / 2,
        surface->w,
        surface->h
    };
}

/**
 * Add on top of the current text container
 */
void TextField::addToTextContainer(std::string toAdd){
    this->textContainer.append(toAdd);
}
/**
 * Remove a singular char from the text container
 */
void TextField::removeFromTextContainer(){
    if(this->textContainer.empty()){
        return;
    }
    this->textContainer = this->textContainer.substr(0, this->textContainer.size() - 1);
}
/**
 * Clear the text container
 */
void TextField::clearTextContainer(){
    this->textContainer.clear();
}

bool TextField::isInside(int x, int y){
    if(this->visible){
        return (this->rect.x <= x && (this->rect.x + this->rect.w) >= x)
            && (this->rect.y <= y && (this->rect.y + this->rect.h) >= y);
    }
    return false;
}