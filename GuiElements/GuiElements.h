#include "TextField.h"
#include "TextButton.h"
#include "Label.h"
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

        /** Get all the text fiels that was added to guiTextFields */
        std::vector<TextField*> getTextFieldList() const;
        /** Add to the guiTextFields a new textField */
        void addTextField(TextField* textField);

        std::vector<TextButton*> getTextButtonsList() const;
        void addTextButton(TextButton* textButton);

        std::vector<Label*> getLabelsList() const;
        void addLabel(Label* label);

        /** Render all the elements that was added to this object */
        void renderAll(SDL_Renderer* renderer);

        // Old logic (to delete soon)
        // virtual void render(SDL_Renderer* renderer) {};
        // virtual bool isInside(int x, int y) {return NULL;};
    private: 
        int WINDOW_WIDTH;
        int WINDOW_HEIGHT;

        std::vector<TextField*> guiTextFields;
        std::vector<TextButton*> guiTextButtons;
        std::vector<Label*> guiLabels;
        // TODO add other gui elements in their own vectors
};
