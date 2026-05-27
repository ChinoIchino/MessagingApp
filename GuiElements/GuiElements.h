#include "Container.h"
#include <vector>

/**
 * Container for all gui elements.
 * Used for all gui management.
 */
class GuiElements{
    public:
        enum GuiGroup{
            LOGIN_GUI_GROUP,
            MAINMENU_GUI_GROUP
        };
        
        GuiElements(SDL_Renderer* renderer, const int WINDOW_WIDTH, const int WINDOW_HEIGHT);
        ~GuiElements();

        /** 
         * Set the input handler and the visibility to true and render the choosen gui group.
         * Set every other groups to false. 
         */
        void changeRender(GuiGroup screenToRender);
        /** 
         * Render all the elements that was added to this object
         * Ignore every item with this.visible = false 
         */
        void renderAll();

        void handleEvent(SDL_Event event);

        /** @return The center of the window based on WINDOW_WIDTH */
        int getCenterX() const{
            return this->WINDOW_WIDTH / 2;
        }
        /** @return The center of the window based on WINDOW_HEIGHT */
        int getCenterY() const{
            return this->WINDOW_HEIGHT / 2;
        }

        int getScreenWidth() const{
            return this->WINDOW_WIDTH;
        }
        int getScreenHeight() const{
            return this->WINDOW_HEIGHT;
        }

        std::vector<Container*> getContainerList() const;
        void addContainer(Container* container);


    private: 
        SDL_Rect rect;
        SDL_Color backgroundColor;

        int WINDOW_WIDTH;
        int WINDOW_HEIGHT;

        SDL_Texture* leftSideOutline;
        SDL_Texture* rightSideOutline;
        SDL_Texture* middleOutline;

        SDL_Renderer* renderer;

        GuiGroup currentDisplayedGroup;

        std::vector<Container*> guiContainers;
};
