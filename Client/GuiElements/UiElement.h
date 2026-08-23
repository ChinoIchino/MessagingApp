#include <SDL2/SDL.h>

class UiElement{
    protected:
        int id;

    public:
        virtual ~UiElement() = default;

        virtual int getId() const = 0;
        virtual SDL_Rect getRect() const = 0;

        virtual void moveTo(int x, int y) = 0;
};