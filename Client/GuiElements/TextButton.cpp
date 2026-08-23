// #include "TextButton.h"
#include "Container.h"
#include <iostream>

TextButton::TextButton(int id, Container* parent, SDL_Rect rect, SDL_Color backgroundColor, std::string textContainer, const char* filePathToFont, SDL_Color fontColor, void (*reactFunction)(void* arg), void* arg){
    this->parent = parent;

    this->id = id;

    this->rect = rect;
    this->backgroundColor = backgroundColor;
    
    int tempRed = this->backgroundColor.r + 50;
    int tempGreen = this->backgroundColor.g + 50;
    int tempBlue = this->backgroundColor.b + 50;
    if(tempRed > 255){
        tempRed = 255;
    }
    if(tempGreen > 255){
        tempGreen = 255;
    }
    if(tempBlue > 255){
        tempBlue = 255;
    }
    this->selectedBackgroundColor = {(Uint8)(tempRed), (Uint8)(tempGreen), (Uint8)(tempBlue), this->backgroundColor.a};
    
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

    this->hovering = false;
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

void TextButton::loadTexture(SDL_Renderer* renderer){
    this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
    this->textRect = {
        this->rect.x + (this->rect.w - this->surface->w) / 2,
        this->rect.y + surface->h / 2,
        surface->w,
        surface->h
    };

    // Give them a position and size (7 because the textures are 7 pixels wide)
    this->leftTexRect = {this->rect.x, this->rect.y, 7, this->rect.h};
    this->rightTexRect = {this->rect.x + this->rect.w - 7 , this->rect.y, 7, this->rect.h};
    this->middleTexRect = {this->rect.x + 7, this->rect.y, this->rect.w - 14, this->rect.h};
}

bool TextButton::isInside(int x, int y){
    if(!this->visible){
        return false;
    }

    if((this->rect.x <= x && (this->rect.x + this->rect.w) >= x)
        && (this->rect.y <= y && (this->rect.y + this->rect.h) >= y)){
        return true;
    }
    return false;
}
bool TextButton::isPressed(int x, int y){
    if(!this->visible){
        return false;
    }
    
    if((this->rect.x <= x && (this->rect.x + this->rect.w) >= x)
        && (this->rect.y <= y && (this->rect.y + this->rect.h) >= y)
    ){
        // std::cout << "Button got pressed" << std::endl;
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

    if(this->hovering){
        SDL_SetRenderDrawColor(renderer, this->selectedBackgroundColor.r, this->selectedBackgroundColor.g, this->selectedBackgroundColor.b, this->selectedBackgroundColor.a);
        SDL_RenderFillRect(renderer, &this->rect);
    }else{
        SDL_SetRenderDrawColor(renderer, this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a);
        SDL_RenderFillRect(renderer, &this->rect);
    }

    // If the surface is null, stopping the render before it crashes
    if(this->texture == NULL){
        std::cout << "TextButton::render: this->texture is NULL, Stopping the render" << std::endl;
        return;
    }
    
    SDL_RenderCopy(renderer, this->texture, NULL, &this->textRect);
}
void TextButton::renderOutline(SDL_Renderer* renderer, SDL_Texture* texLeftSide, SDL_Texture* texRigthSide, SDL_Texture* texMiddleSide){
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

SDL_Rect TextButton::getRect() const{
    return this->rect;
}
int TextButton::getId() const{
    return this->id;
}

bool TextButton::isVisible() const{
    return this->visible;
}
void TextButton::setIsVisible(bool isVisible){
    this->visible = isVisible;
}
void TextButton::setIsHovering(bool isHovering){
    this->hovering = isHovering;
}

TextButton* TextButton::snapToTop(int padding){
    int width = this->rect.w;
    int height = this->rect.h;

    this->rect = {this->parent->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X) - width / 2, this->parent->claimTopPosition(height, padding), width, height};

    return this;
}
TextButton* TextButton::snapToTop(){
    int width = this->rect.w;
    int height = this->rect.h;

    this->rect = {this->parent->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X) - width / 2, this->parent->claimTopPosition(height), width, height};

    return this;
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
