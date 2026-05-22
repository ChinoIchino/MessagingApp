#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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

        void render(SDL_Renderer* renderer);

        SDL_Rect getRect();

        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToLeftOf(SDL_Rect rectToSnap, int padding);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToRightOf(SDL_Rect rectToSnap, int padding);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToTopOf(SDL_Rect rectToSnap, int padding);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToBottomOf(SDL_Rect rectToSnap, int padding);

        /** Automatically activate the function in the reactFunction attribut */
        bool isPressed(int x, int y);
    private:
        SDL_Rect rect;
        SDL_Color backgroundColor;

        TTF_Font* font;
        SDL_Color fontColor;

        SDL_Surface* surface;
        SDL_Texture* texture;

        std::string textContainer;

        /** Function that will be called via the isPressed */
        void (*reactFunction)(void* arg);
        /** Attribut of the reactFunction */
        void* arg;
};