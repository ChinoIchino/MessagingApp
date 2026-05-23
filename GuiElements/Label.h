#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

/** A label without a background to display static information */
class Label{
    public:
        Label(
            SDL_Rect rect,
            std::string textContainer, 
            const char* filePathToFont,
            SDL_Color fontColor
        );
        ~Label();

        void render(SDL_Renderer* renderer);

        SDL_Rect getRect() const;

        bool isVisible() const;
        void setVisibility(bool isVisible);

        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToLeftOf(SDL_Rect rectToSnap, int padding);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToRightOf(SDL_Rect rectToSnap, int padding);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToTopOf(SDL_Rect rectToSnap, int padding);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToBottomOf(SDL_Rect rectToSnap, int padding);

    private:
        SDL_Rect rect;
        SDL_Color backgroundColor;

        TTF_Font* font;
        SDL_Color fontColor;

        SDL_Surface* surface;
        SDL_Texture* texture;

        std::string textContainer;

        bool visible;
};