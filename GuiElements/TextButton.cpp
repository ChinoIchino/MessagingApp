#include "TextButton.h"
#include <iostream>

TextButton::TextButton(SDL_Rect rect, SDL_Color backgroundColor, std::string textContainer, const char* filePathToFont, SDL_Color fontColor, void (*reactFunction)(void* arg), void* arg){
    this->rect = rect;
    this->backgroundColor = backgroundColor;
    
    this->font = TTF_OpenFont(filePathToFont, this->rect.h / 2);

    if(!this->font){
        std::cout << "TextButton font failed: "<< TTF_GetError() << std::endl;
    }

    this->fontColor = fontColor;
    this->textContainer = textContainer;

    this->surface = TTF_RenderText_Solid(this->font, this->textContainer.c_str(), this->fontColor);
    this->texture = nullptr;

    this->reactFunction = reactFunction;
    this->arg = arg;

    this->visible = true;
}
TextButton::~TextButton(){
    if(this->surface != nullptr){
        SDL_FreeSurface(this->surface);
    }
    if(this->texture != nullptr){
        SDL_DestroyTexture(this->texture);
    }
}

bool TextButton::isPressed(int x, int y){
    if(!this->visible){
        return false;
    }
    
    if((this->rect.x <= x && (this->rect.x + this->rect.w) >= x)
        && (this->rect.y <= y && (this->rect.y + this->rect.h) >= y)
    ){
        std::cout << "Button got pressed" << std::endl;
        if(this->reactFunction != NULL){
            this->reactFunction(this->arg);
        }
        return true;
    }
    return false;
}

void TextButton::render(SDL_Renderer* renderer){
    if(!this->visible){
        return;
    }

    SDL_SetRenderDrawColor(renderer, this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a);
    SDL_RenderFillRect(renderer, &this->rect);

    if(this->textContainer.empty()){
        return;
    }

    // If the surface is null, stopping the render before it crashes
    if(this->surface == nullptr){
        std::cout << "TextButton::render: this->surface is NULL, Stopping the render" << std::endl;
        return;
    }
    // Delete the texture from the last frame
    if(this->texture != nullptr){
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
    // Create a new texture for the current frame
    this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);

    if(this->texture == nullptr){
        std::cout << "TextButton::render: this->texture is NULL, Stopping the render" << std::endl;
        return;
    }

    SDL_Rect dst = {
        this->rect.x + (this->rect.w - this->surface->w) / 2,
        this->rect.y + surface->h / 2,
        surface->w,
        surface->h
    };

    // SDL_Texture* imageTexture = IMG_LoadTexture(renderer, "Ressource/Img/buttonTextureV2.png");
    // if(imageTexture == NULL){
    //     std::cout << "Error loading image" << std::endl;
    // }

    SDL_RenderCopy(renderer, this->texture, NULL, &dst);
    // SDL_RenderCopy(renderer, imageTexture, NULL, &this->rect);
}

SDL_Rect TextButton::getRect(){
    return this->rect;
}

bool TextButton::isVisible() const{
    return this->visible;
}
void TextButton::setIsVisible(bool isVisible){
    this->visible = isVisible;
}

void TextButton::moveTo(int x, int y){
    int width = this->rect.w;
    int height = this->rect.h;

    this->rect = {x, y, width, height};
}
void TextButton::snapToLeftOf(SDL_Rect rectToSnap, int padding){
    int height = this->rect.h;
    int width = this->rect.w;

    this->rect = {
        rectToSnap.x - width - padding,
        rectToSnap.y,
        width,
        height
    };
}
void TextButton::snapToRightOf(SDL_Rect rectToSnap, int padding){
    int height = this->rect.h;
    int width = this->rect.w;

    this->rect = {
        rectToSnap.x + rectToSnap.w + padding,
        rectToSnap.y,
        width,
        height
    };
}
void TextButton::snapToBottomOf(SDL_Rect rectToSnap, int padding){
    int height = this->rect.h;
    int width = this->rect.w;

    this->rect = {
        rectToSnap.x,
        rectToSnap.y + height + padding,
        width,
        height
    };
}
void TextButton::snapToTopOf(SDL_Rect rectToSnap, int padding){
    int height = this->rect.h;
    int width = this->rect.w;

    this->rect = {
        rectToSnap.x,
        rectToSnap.y - height - padding,
        width,
        height
    };
}
