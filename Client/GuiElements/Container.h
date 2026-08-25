#include "UiElement.h"
#include "TextField.h"
#include "TextButton.h"
#include "Label.h"
#include "ScrollPane.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <string>
#include <vector>


class Container{
    public:
        enum GuiLayout{
            DEFAULT,
            CENTERED
        };

        enum AbsolutePositionX{
            LEFT_X,
            MIDDLE_X,
            RIGHT_X
        };
        enum AbsolutePositionY{
            TOP_Y,
            MIDDLE_Y,
            BOTTOM_Y
        };

        Container(
            int windowHeight,
            int windowWidth,
            bool isRounded,
            SDL_Color backgroundColor,
            SDL_Color guiElementsBackgroundColor,
            std::string filePathToFont,
            SDL_Color fontColor
        );
        ~Container();

        void loadOutline(SDL_Texture* leftSideOutline, SDL_Texture* rightSideOutline, SDL_Texture* middleOutline);
        void loadAllTextures(SDL_Renderer* renderer);

        void render(SDL_Renderer* renderer);

        std::vector<TextField*> getTextFieldList() const;
        /**
         * Create and add a TextField into the container.
         * The text field created have the position 0 0. It need then to be moved manually.
         * @return the TextField* that was just created and added to the container
         */
        TextField* addTextField(int width, int height);
        /**
         * Create and add a TextField into the container. With a chosen size based on the container size.
         * The text field created have the position 0 0. It need then to be moved manually.
         * @return the TextField* that was just created and added to the container
         */
        TextField* addTextField();

        std::vector<TextButton*> getTextButtonsList() const;
        /**
         * Create and add a TextButton into the container.
         * The text button created have the position 0 0. It need then to be moved manually.
         * @return the TextButton* that was just created and added to the container
         */
        TextButton* addTextButton(int width, int height, std::string displayText, void (*reactFunction)(void* arg), void* arg);
        /**
         * Create and add a TextButton into the container. With a chosen size based on the container size.
         * The text button created have the position 0 0. It need then to be moved manually.
         * @return the TextButton* that was just created and added to the container
         */
        TextButton* addTextButton(std::string displayText, void (*reactFunction)(void* arg), void* arg);

        std::vector<Label*> getLabelsList() const;
        /**
         * Create and add a Label into the container.
         * The label created have the position 0 0. It need then to be moved manually.
         * @return the Label* that was just created and added to the container
         */
        Label* addLabel(std::string displayedText, int fontSize);
        /**
         * Create and add a Label into the container. With a chosen font size based on the container size.
         * The label created have the position 0 0. It need then to be moved manually.
         * @return the Label* that was just created and added to the container
         */
        Label* addLabel(std::string displayedText);

        ScrollPane* addScrollPane();

        int getCenterX();
        int getCenterY();

        int getWidth();
        int getHeight();

        bool handleMouseInput(int x, int y);
        bool handleMouseScroll(int verticalScrollAmm);
        bool handleTextInput(std::string input, SDL_Renderer* renderer);
        bool handleKeyInput(SDL_Keycode key, SDL_Renderer* renderer);
        bool handleMouseMotion(int x, int y);

        bool isVisible() const;
        void setIsVisible(bool isVisible);

        bool isHandlingInput() const;
        void setHandleInput(bool isHandling);

        void setRectSize(float width, float height);

        void setGuiLayout(GuiLayout newLayout);

        int getAbsolutePositionX(AbsolutePositionX absX);
        int getAbsolutePositionY(AbsolutePositionY absY);

        int claimTopPosition(int itemHeight, int padding);
        int claimTopPosition(int itemHeight);

        
        private:
        const int WINDOW_HEIGHT;
        const int WINDOW_WIDTH;
        
        /**
         * Background above the one made by GuiElements
         */
        SDL_Rect rect;
        SDL_Color backgroundColor;
        SDL_Color guiBackgroundColor;

        GuiLayout chosenLayout;
        
        int ammountOfItems;
        int availableTopPosition;
        
        SDL_Texture* leftSideOutline;
        SDL_Texture* rightSideOutline;
        SDL_Texture* middleOutline;
        
        std::string fontPath;
        SDL_Color fontColor;

        std::vector<TextField*> guiTextFields;
        std::vector<TextButton*> guiTextButtons;
        std::vector<Label*> guiLabels;
        std::vector<ScrollPane*> guiScrollPanes;
        
        bool rounded;
        bool visible;
        bool handleInput;

        UiElement* searchById(int id);
        
        /**
         * When a new item is added in the container and the GuiLayout isn't set to DEFAULT,
         * the container clean and put again every gui elements to make a place for the new item.
         */
        void reformatGui();
};