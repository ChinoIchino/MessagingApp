#include "TextField.h"
#include "TextButton.h"
#include "Label.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>


class Container{
    public:
        Container(
            SDL_Rect rect,
            SDL_Color backgroundColor
        );
        ~Container();

        void render(SDL_Renderer* renderer);

        std::vector<TextField*> getTextFieldList() const;
        void addTextField(TextField* textField);

        std::vector<TextButton*> getTextButtonsList() const;
        void addTextButton(TextButton* textButton);

        std::vector<Label*> getLabelsList() const;
        void addLabel(Label* label);

        int getCenterX();
        int getCenterY();

        int getWidth();
        int getHeight();

        bool handleMouseInput(int x, int y);

        bool isVisible() const;
        void setIsVisible(bool isVisible);

        bool isHandlingInput() const;
        void setHandleInput(bool isHandling);

    private:
        SDL_Rect rect;
        SDL_Color backgroundColor;

        std::vector<TextField*> guiTextFields;
        std::vector<TextButton*> guiTextButtons;
        std::vector<Label*> guiLabels;

        bool visible;
        bool handleInput;
};