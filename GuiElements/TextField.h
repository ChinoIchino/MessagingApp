#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class TextField{
    public:
        TextField(SDL_Rect rect, SDL_Color bgColor, const char* filePathToFont, SDL_Color fontColor);
        ~TextField();

        void addToTextContainer(std::string toAdd);
        void removeFromTextContainer();
        void clearTextContainer();

        bool getIsSelected();
        void setIsSelected(bool isSelected);

        SDL_Rect getRect();

        void render(SDL_Renderer* renderer);

        bool isInside(int x, int y);

    private:
        /** Position and size of the TextField*/
        SDL_Rect rect;

        // All the variable used to display the font
        TTF_Font* font;
        SDL_Surface* surface;
        SDL_Texture* texture;

        /** Color of the text inside the TextField */
        SDL_Color fontColor;
        /** Color of the background in the TextField */
        SDL_Color backgroundColor;

        /** String that represent the text inside the TextField */
        std::string textContainer;

        /** If the user have selected this */
        bool isSelected;
};