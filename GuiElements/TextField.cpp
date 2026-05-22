#include "TextField.h"
#include <iostream>

TextField::TextField(SDL_Rect rect, SDL_Color bgColor, const char* filePathToFont, SDL_Color fontColor){
    this->rect = rect;
    this->backgroundColor = bgColor;

    this->font = TTF_OpenFont(filePathToFont, this->rect.h / 2);

    if(!this->font){
        std::cout << "TextField font failed: "<< TTF_GetError() << std::endl;
    }

    this->fontColor = fontColor;

    this->surface = nullptr;
    this->texture = nullptr;
}

TextField::~TextField(){
    if(this->surface != nullptr){
        SDL_FreeSurface(this->surface);
    }
    if(this->texture != nullptr){
        SDL_DestroyTexture(this->texture);
    }
}

bool TextField::getIsSelected(){
    return this->isSelected;
}
void TextField::setIsSelected(bool newStatus){
    this->isSelected = newStatus;
}

SDL_Rect TextField::getRect(){
    return this->rect;
}

void TextField::render(SDL_Renderer* renderer){
    // std::cout << "TextField Render got called" << std::endl;
    if(this->surface != nullptr){
        SDL_FreeSurface(this->surface);
        this->surface = nullptr;
    }
    if(this->texture != nullptr){
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }

    SDL_SetRenderDrawColor(renderer, this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a);
    SDL_RenderFillRect(renderer, &this->rect);

    // If there is no text to be processed, there is no need for the function to continue
    if(this->textContainer.empty()){
        return;
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
            visibleText.c_str(),
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
    
    if(this->surface == NULL){
        std::cout << "TextField::render: this->surface is NULL, Stopping the render" << std::endl;
        return;
    }
    if(this->texture == NULL){
        std::cout << "TextField::render: this->texture is NULL, Stopping the render" << std::endl;
        return;
    }

    SDL_Rect dst = {
        this->rect.x + offset + 5,
        this->rect.y + surface->h / 2,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(renderer, this->texture, NULL, &dst);
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
    // if(this->textContainer.size() > 1){
    //     this->textContainer = this->textContainer.substr(0, this->textContainer.size() - 1);
    // }else{
    //     this->textContainer.clear();
    // }
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
    return (this->rect.x <= x && (this->rect.x + this->rect.w) >= x)
        && (this->rect.y <= y && (this->rect.y + this->rect.h) >= y);
}