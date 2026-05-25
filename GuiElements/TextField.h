#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class TextField{
    public:
        TextField(SDL_Rect rect, SDL_Color bgColor, const char* filePathToFont, SDL_Color fontColor);
        ~TextField();

        /**
         * @param toAdd string of text that will be added to the current textContainer
         * This function render instantly after updating the textContainer.
         */
        void addToTextContainer(std::string toAdd);
        /** This function render instantly after updating the textContainer. */
        void removeFromTextContainer();
        /** This function render instantly after updating the textContainer. */
        void clearTextContainer();

        bool isSelected() const;
        void setIsSelected(bool isSelected);

        std::string getTextContainer() const;
        SDL_Rect getRect() const;

        bool isVisible() const;
        void setIsVisible(bool isVisible);

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

        bool visible;
        /** If the user have selected this */
        bool selected;
};