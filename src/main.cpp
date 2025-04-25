#include <SFML/Graphics.hpp>
#include "renderer.h"
#include "camera/view.h"

int main1()
{
    renderer r;
    r.set_render_fps_limit(60);
    r.set_physics_fps_limit(30);
    r.run();
}
