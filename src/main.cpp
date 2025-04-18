#include <SFML/Graphics.hpp>
#include "renderer.h"

int main()
{
    renderer r;
    r.set_framerate_limit(60);
    r.run();
}
