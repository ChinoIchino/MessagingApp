#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <string>

class TextButton{
    public:
        TextButton(
            SDL_Rect rect,
            SDL_Color backgroundColor,
            std::string textContainer, 
            const char* filePathToFont,
            SDL_Color fontColor,
            void (*reactFunction)(void* arg),
            void* arg
        );
        ~TextButton();

        void loadTexture(SDL_Renderer* renderer);

        void render(SDL_Renderer* renderer);
        /** Used to render related textures. Separated from the render function, because there is no reason to re-render it later*/
        void renderOutline(SDL_Renderer* renderer, SDL_Texture* leftSideOutline, SDL_Texture* rightSideOutline, SDL_Texture* middleOutline);

        SDL_Rect getRect();

        bool isVisible() const;
        void setIsVisible(bool isVisible);
        void setIsHovering(bool isHovering);

        /** Move to a absolute position based on the attributs */
        void moveTo(int x, int y);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToLeftOf(SDL_Rect rectToSnap, int padding);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToRightOf(SDL_Rect rectToSnap, int padding);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToTopOf(SDL_Rect rectToSnap, int padding);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToBottomOf(SDL_Rect rectToSnap, int padding);

        bool isInside(int x, int y);
        /** Automatically activate the function in the reactFunction attribut */
        bool isPressed(int x, int y);
    private:
        SDL_Rect rect;
        SDL_Color backgroundColor;
        SDL_Color selectedBackgroundColor;

        TTF_Font* font;
        SDL_Color fontColor;

        SDL_Surface* surface;
        SDL_Texture* texture;

        std::string textContainer;
        SDL_Rect textRect;

        SDL_Rect leftTexRect;
        SDL_Rect rightTexRect;
        SDL_Rect middleTexRect;

        bool hovering;
        bool visible;

        /** Function that will be called via the isPressed */
        void (*reactFunction)(void* arg);
        /** Attribut of the reactFunction */
        void* arg;
};