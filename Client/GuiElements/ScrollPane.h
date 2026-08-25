// #include <SDL2/SDL.h>
// #include <SDL2/SDL_ttf.h>
// #include <SDL2/SDL_image.h>

#include <vector>

class ScrollPane{
    public:
        ScrollPane(int posX, int posY, int width, int height, std::string fontPath);
        ~ScrollPane();

        void addMessage(std::string message);
        // void reformatMessagesDisplayed();

        void render(SDL_Renderer* renderer);

        void handleEvent();

        SDL_Rect getRect() const;

        bool isHovering() const;
        void setIsHovering(bool isHovering);

        bool isInside(int x, int y);

        void modifyScrollOffset(int toAdd);

    protected:

    private:
        SDL_Rect rect;
        SDL_Rect scrollBarBackground;
        SDL_Rect scrollBar;

        SDL_Color rectColor;
        SDL_Color scrollBarBackgroundColor;
        SDL_Color scrollBarColor;

        std::string fontPath;
        TTF_Font* font;
        SDL_Color fontColor = {255, 255, 255};

        std::vector<Label*> messagesContainer;
        int totalMessagesContainerSize = 0;

        int scrolledOffset = 0;

        bool visible = true;
        bool scrollBarVisible = true;
        bool hovering = false;

};