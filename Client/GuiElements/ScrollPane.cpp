#include "Container.h"
#include <iostream>

// FIXME Long messages get clipped

ScrollPane::ScrollPane(int posX, int posY, int width, int height, std::string fontPath){
    this->rect = {posX, posY, width - 25, height};
    
    this->scrollBarBackground = {posX + width - 25, posY, 25, height};
    this->scrollBar = {posX + width - 25, posY + height - 25, 25, 30};

    this->rectColor = {100, 100, 100, 255}; //TODO Modify those variable with attributs
    this->scrollBarBackgroundColor = {200, 200, 200, 255};
    this->scrollBarColor = {150, 150, 150, 255};

    this->fontPath = fontPath;
    this->font = TTF_OpenFont(fontPath.c_str(), 75);

    if(!this->font){
        std::cout << "label font failed: "<< TTF_GetError() << std::endl;
    }
    this->fontColor = {255, 255, 255};

    this->scrolledOffset = ((this->totalMessagesContainerSize - this->rect.h + 49) / 50);
}

ScrollPane::~ScrollPane(){}

void ScrollPane::render(SDL_Renderer* renderer){
    if(!this->visible){
        return;
    }

    SDL_SetRenderDrawColor(renderer, this->rectColor.r, this->rectColor.g, this->rectColor.b, this->rectColor.a);
    SDL_RenderFillRect(renderer, &this->rect);

    if(this->scrollBarVisible){
        SDL_SetRenderDrawColor(renderer, this->scrollBarBackgroundColor.r, this->scrollBarBackgroundColor.g, this->scrollBarBackgroundColor.b, this->scrollBarBackgroundColor.a);
        SDL_RenderFillRect(renderer, &this->scrollBarBackground);
    
        SDL_SetRenderDrawColor(renderer, this->scrollBarColor.r, this->scrollBarColor.g, this->scrollBarColor.b, this->scrollBarColor.a);
        SDL_RenderFillRect(renderer, &this->scrollBar);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderSetClipRect(renderer, &this->rect);

    int startOfRender = this->scrolledOffset * 50;
    int currentPos = 0;
    int labelHeight;
    for(Label* label: this->messagesContainer){
        if(label->isWrapped()){
            labelHeight = label->getSurface().h;
        }else{
            labelHeight = label->getRect().h;
        }

        if((currentPos + labelHeight) < startOfRender){
            currentPos += labelHeight;
            continue;
        }
        
        if((currentPos - startOfRender) > this->rect.h){
            break;
        }
        
        int posX = this->rect.x + 5;
        int posY = this->rect.y + currentPos - startOfRender;

        // Temporarily move label to its calculated screen position and draw
        label->moveTo(posX, posY);
        label->render(renderer);
        
        currentPos += labelHeight;

    }

    SDL_RenderSetClipRect(renderer, NULL);
}

void ScrollPane::addMessage(std::string message){
    Label* label = new Label(0, NULL, {this->rect.x, totalMessagesContainerSize, this->rect.w, 75}, message, this->fontPath.c_str(), this->fontColor);
    
    int messageWidth = 0;
    TTF_SizeText(
        this->font,
        message.c_str(),
        &messageWidth,
        NULL
    );
    
    std::cout << "Message width : " << messageWidth << std::endl;
    if(messageWidth > this->rect.w){
        label->setWrapLength(this->rect.w);
        this->totalMessagesContainerSize += label->getSurface().h; //+13 is the offset between messages
        std::cout << "1Got the ammount = " << this->totalMessagesContainerSize << std::endl;
    }else{
        this->totalMessagesContainerSize += label->getRect().h;
        std::cout << "2Got the ammount = " << this->totalMessagesContainerSize << std::endl;
    }
    this->messagesContainer.push_back(label);

    // std::cout << "Adding to the messageContainerSize : " << label->getRect().h << " // Surface size : " << label->getSurface().h << std::endl;

    int maxScroll = (this->totalMessagesContainerSize - this->rect.h + 49) / 50;
    if(maxScroll != 0){
        this->scrollBar.h = this->rect.h / maxScroll;
    }else{
        this->scrollBar.h = this->rect.h;
    }

    //TODO Change the size of the label, will prolly take place inside the constructor of the label
}

SDL_Rect ScrollPane::getRect() const{
    return this->rect;
}

bool ScrollPane::isHovering() const{
    return this->hovering;
}
void ScrollPane::setIsHovering(bool isHovering){
    this->hovering = isHovering;
}

bool ScrollPane::isInside(int x, int y){
    if(!this->visible){
        return false;
    }

    if((this->rect.x <= x && (this->rect.x + this->rect.w) >= x)
        && (this->rect.y <= y && (this->rect.y + this->rect.h) >= y)){
        return true;
    }
    return false;
}

void ScrollPane::modifyScrollOffset(int toAdd){
    this->scrolledOffset -= toAdd;
    if(this->scrolledOffset < 0){
        this->scrolledOffset = 0;
    }

    int maxScroll = (this->totalMessagesContainerSize - this->rect.h + 49) / 50;
    if(this->scrolledOffset > maxScroll){
        this->scrolledOffset += toAdd;
    }

    std::cout << "Scrolled offset = " << this->scrolledOffset << " // Max Scroll : " << maxScroll << std::endl;

    if(this->scrollBarVisible){
        if(this->scrolledOffset != 0){
            this->scrollBar.y = this->rect.y + (this->rect.h / (maxScroll + 1) * this->scrolledOffset);
        }else{
            this->scrollBar.y = this->rect.y;
        }
        

    }
}