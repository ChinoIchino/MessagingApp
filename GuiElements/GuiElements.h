#include "Container.h"
#include <vector>

/**
 * Container for all gui elements.
 * Used for all gui management.
 */
class GuiElements{
    public:
        GuiElements(const int WINDOW_WIDTH, const int WINDOW_HEIGHT);
        ~GuiElements();

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

        /** Render all the elements that was added to this object */
        void renderAll(SDL_Renderer* renderer);

    private: 
        int WINDOW_WIDTH;
        int WINDOW_HEIGHT;

        std::vector<Container*> guiContainers;
};
