// #include "Label.h"
#include "Container.h"
#include <iostream>

Label::Label(Container* parent, SDL_Rect rect, std::string textContainer, const char* filePathToFont, SDL_Color fontColor){
    this->parent = parent;
    
    this->rect = rect;

    this->font = TTF_OpenFont(filePathToFont, this->rect.h / 2);

    if(!this->font){
        std::cout << "label font failed: "<< TTF_GetError() << std::endl;
    }

    this->fontColor = fontColor;

    this->textContainer = textContainer;

    this->surface = TTF_RenderText_Solid(this->font, this->textContainer.c_str(), this->fontColor);
    this->texture = nullptr;

    this->visible = true;
}

Label::~Label(){
    if(this->surface != nullptr){
        SDL_FreeSurface(this->surface);
    }
    if(this->texture != nullptr){
        SDL_DestroyTexture(this->texture);
    }
}

void Label::render(SDL_Renderer* renderer){
    // If the visibility is set to false do not render
    if(!this->visible){
        return;
    }

    if(this->surface == nullptr){
        std::cout << "Label::render: this->surface is NULL, Stopping the render" << std::endl;
        return;
    }

    if(this->texture != nullptr){
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
    this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
    if(this->texture == nullptr){
        std::cout << "Label::render: this->texture is NULL, Stopping the render" << std::endl;
        return;
    }

    SDL_Rect dst = {
        this->rect.x + (this->rect.w - this->surface->w) / 2,
        this->rect.y + surface->h / 2,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(renderer, this->texture, NULL, &dst);
}

SDL_Rect Label::getRect() const{
    return this->rect;
}

bool Label::isVisible() const{
    return this->visible;
}
void Label::setVisibility(bool isVisible){
    this->visible = isVisible;
}

Label* Label::snapToTop(int padding){
    int width = this->rect.w;
    int height = this->rect.h;

    this->rect = {this->parent->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X) - width / 2, this->parent->claimTopPosition(height, padding), width, height};

    return this;
}
Label* Label::snapToTop(){
    int width = this->rect.w;
    int height = this->rect.h;

    this->rect = {this->parent->getAbsolutePositionX(Container::AbsolutePositionX::MIDDLE_X) - width / 2, this->parent->claimTopPosition(height), width, height};

    return this;
}

void Label::moveTo(int x, int y){
    int height = this->rect.h;
    
    int widthOfText = 0;
    TTF_SizeText(
            this->font,
            this->textContainer.c_str(),
            &widthOfText,
            NULL
        );
    
    this->rect = {x, y, widthOfText, height};
}
Label* Label::snapLeftOf(SDL_Rect rectToSnap, int padding){
    // std::cout << "Old label Rect: " << this->rect.x << " " << this->rect.y << " // " << this->rect.w << " " << this->rect.h << std::endl;
    int height = this->rect.h;
    
    int widthOfText = 0;
    TTF_SizeText(
            this->font,
            this->textContainer.c_str(),
            &widthOfText,
            NULL
        );

    this->rect = {
        rectToSnap.x - widthOfText - padding,
        rectToSnap.y,
        widthOfText,
        height
    };
    // std::cout << "rectToSnap : " << rectToSnap.x << " " << rectToSnap.y << " // " << rectToSnap.w << " " << rectToSnap.h << std::endl;
    // std::cout << "New Label Rect: " << this->rect.x << " " << this->rect.y << " // " << this->rect.w << " " << this->rect.h << std::endl;
    // std::cout << "rectToSnap.x - offsetOfLabel : " << rectToSnap.x << " - " << offsetOfLabel << " = " << (rectToSnap.x - offsetOfLabel) << std::endl;

    return this;

}
void Label::snapToRightOf(SDL_Rect rectToSnap, int padding){
    int height = this->rect.h;
    
    int widthOfText = 0;
    TTF_SizeText(
            this->font,
            this->textContainer.c_str(),
            &widthOfText,
            NULL
        );

    this->rect = {
        rectToSnap.x + rectToSnap.w + padding,
        rectToSnap.y,
        widthOfText,
        height
    };
}
void Label::snapToBottomOf(SDL_Rect rectToSnap, int padding){
    int height = this->rect.h;
    
    int widthOfText = 0;
    TTF_SizeText(
            this->font,
            this->textContainer.c_str(),
            &widthOfText,
            NULL
        );

    this->rect = {
        rectToSnap.x,
        rectToSnap.y + height + padding,
        widthOfText,
        height
    };
}
void Label::snapToTopOf(SDL_Rect rectToSnap, int padding){
    int height = this->rect.h;
    
    int widthOfText = 0;
    TTF_SizeText(
            this->font,
            this->textContainer.c_str(),
            &widthOfText,
            NULL
        );

    this->rect = {
        rectToSnap.x,
        rectToSnap.y - height - padding,
        widthOfText,
        height
    };
}

Label* Label::centerGroup(TextField* tf, int padding){
    tf->moveTo(tf->getRect().x + tf->getRect().w / 2, tf->getRect().y);
    this->snapLeftOf(tf->getRect(), 10);
    
    return this;
}
Label* Label::centerGroup(TextField* tf){
    tf->moveTo(tf->getRect().x + tf->getRect().w / 2, tf->getRect().y);
    this->snapLeftOf(tf->getRect(), 0);
    
    return this;
}