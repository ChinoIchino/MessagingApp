#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Container;

/** A label without a background to display static information */
class Label{
    public:
        Label(
            Container* parent,
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

        Label* snapToTop(int padding);
        Label* snapToTop();

        /** Move to a absolute position based on the attributs */
        void moveTo(int x, int y);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        Label* snapLeftOf(SDL_Rect rectToSnap, int padding);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToRightOf(SDL_Rect rectToSnap, int padding);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToTopOf(SDL_Rect rectToSnap, int padding);
        /** Change the rect attribut based on the position of rectToSnap, padding and the instance size */
        void snapToBottomOf(SDL_Rect rectToSnap, int padding);

        Label* centerGroup(TextField* rect, int padding);
        Label* centerGroup(TextField* rect);

    private:
        Container* parent;

        SDL_Rect rect;
        SDL_Color backgroundColor;

        TTF_Font* font;
        SDL_Color fontColor;

        SDL_Surface* surface;
        SDL_Texture* texture;

        std::string textContainer;

        bool visible;
};