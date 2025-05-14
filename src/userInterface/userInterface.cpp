#include "userInterface.h"

#include <iostream>
#include <cmath>

#include "checkbox.h"
#include "range.h"
#include "dropdown.h"
#include "button.h"
#include "matrix.h"

using namespace std;

UserInterface::UserInterface(sf::Vector2f windowSize) : font("hih.ttf"), lines(sf::PrimitiveType::Lines, 2*4) {
    view.setSize(windowSize);
    view.setCenter(windowSize / 2.f);
    create_elements();
}

void UserInterface::create_elements() {
    sf::Text text_1(font, "Particles", 14);
    text_1.setPosition({5, 5});
    text_1.setStyle(sf::Text::Bold);
    details.push_back(text_1);

    sf::Text text_2(font, "Particle count", 12);
    text_2.setPosition({5, 25});
    details.push_back(text_2);
    elements["particle_count"] = new Range({85, 23}, {62, 19}, 1000, 100, 0, 1000000);

    sf::Text text_3(font, "Particle types", 12);
    text_3.setPosition({5, 45});
    details.push_back(text_3);
    elements["particle_types"] = new Range({85, 43}, {30, 19}, 8, 1, 1, 20, [this]{this->matrix->resize_matrix(this->elements["particle_types"]->value); this->elements["spawn_type"]->max_value = this->elements["particle_types"]->value; this->elements["spawn_type"]->update_shapes();});

    // Matrix
    matrix = new Matrix({5, 65}, {190, 190}, this->elements["particle_types"]->value, 0, 0.25, -1, 1);
    elements["help_matrix"] = new Button({176, 45}, {19, 19}, "?");
    elements["help_matrix"]->tooltip = "Matrix represents attraction force of one particle type (row) to another (column)\nEach of the types is represented by its dedicated color\n\nFor example, if there are particles of two types nearby:\n(Type 1 is circle color to the left from a cell, Type 2 is circle color on top of a cell)\nThen Type 1 particle will get attracted or repelled to Type 2, based on the matrix cell color\n\nBlue cell means particle will be attracted (1)\nGray cell means particle will be neutral (0)\nRed cell means particle will be repelled (-1)\n\nControls (applicable to other UI elements too):\n- Left Click/Scroll up: Increase attraction\n- Right Click/Scroll down: Decrease attraction\n- Middle Click: Reset to 0 (neutral)";


    sf::Text text_01(font, "Preset", 12);
    text_01.setPosition({5, 263});
    details.push_back(text_01);
    elements["matrix_preset"] = new Dropdown({45, 262}, {70, 19}, {"     Random", "      Null", "     Snake", "     Spaghetti", "     Islands"});
    elements["matrix_preset_apply"] = new Button({120, 262}, {40, 19}, "Apply", [this]{this->matrix->matrix_preset(this->elements["matrix_preset"]->value); ;});
    elements["matrix_preset_apply"]->buttonColor = sf::Color(60, 60, 120);
    elements["help_preset"] = new Button({176, 262}, {19, 19}, "?");
    elements["help_preset"]->tooltip = "Choose preset for matrix and then click Apply\n\nControls (applicable to other UI elements too):\n- Left Click/Scroll up: Next value\n- Right Click/Scroll down: Previous value\n- Middle Click: Reset to default value";

    add_line(0, 285);

    sf::Text text_4(font, "Mouse", 14);
    text_4.setPosition({5, 290});
    text_4.setStyle(sf::Text::Bold);
    details.push_back(text_4);

    sf::Text text_02(font, "Radius", 12);
    text_02.setPosition({5, 310});
    details.push_back(text_02);
    elements["mouse_radius"] = new Range({48, 308}, {30, 19}, 100, 5, 5, 1000);

    sf::Text text_5(font, "Drag type", 12);
    text_5.setPosition({5, 330});
    details.push_back(text_5);
    elements["drag_type"] = new Dropdown({65, 328}, {60, 19}, {"     Attract", "     Move"});

    elements["help_drag"] = new Button({176, 328}, {19, 19}, "?");
    elements["help_drag"]->tooltip = "Use Left Mouse Button to attract/move particles within the radius\n\nAttract - Changes velocity of particle by applying a force that pulls them toward the cursor \n                  Works only when unpaused and velocity enabled\n\nMove - Changes position of particle based on distance from cursor\n                Drag force changes curvature, the bigger, the more even\n\nHold Ctrl to visualize radius";

    sf::Text text_6(font, "Drag force", 12);
    text_6.setPosition({5, 350});
    details.push_back(text_6);
    elements["drag_force"] = new Range({65, 348}, {60, 19}, 25, 5, 0, 100);

    sf::Text text_7(font, "Mode", 12);
    text_7.setPosition({5, 370});
    details.push_back(text_7);
    elements["mouse_mode"] = new Dropdown({40, 368}, {60, 19}, {"     Spawn", "     Delete"});
    elements["mouse_mode"]->tooltip = "WIP, disabled";
    elements["mouse_mode"]->disable();

    elements["help_brush"] = new Button({176, 368}, {19, 19}, "?");
    elements["help_brush"]->tooltip = "WIP, disabled\n\nUse Right Mouse Button to Spawn/Delete particles around cursor within the radius\nSet spawn type to 0, to spawn random particles\n\nHold Ctrl to visualize radius";

    sf::Text text_8(font, "Spawn count", 12);
    text_8.setPosition({5, 390});
    details.push_back(text_8);
    elements["spawn_count"] = new Range({80, 388}, {50, 19}, 1, 1, 1, 100);
    elements["spawn_count"]->tooltip = "WIP, disabled";
    elements["spawn_count"]->disable();

    sf::Text text_9(font, "Spawn type", 12);
    text_9.setPosition({5, 410});
    details.push_back(text_9);
    elements["spawn_type"] = new Range({72, 408}, {30, 19}, 1, 1, 1, this->elements["particle_types"]->value);
    elements["spawn_type"]->tooltip = "WIP, disabled";
    elements["spawn_type"]->disable();

    add_line(1, 430);

    sf::Text text_10(font, "Physics", 14);
    text_10.setPosition({5, 435});
    text_10.setStyle(sf::Text::Bold);
    details.push_back(text_10);

    sf::Text text_03(font, "Max FPS", 12);
    text_03.setPosition({5, 455});
    details.push_back(text_03);
    elements["fps_limit"] = new Range({56, 453}, {30, 19}, 60, 5, 0, 500);

    sf::Text text_04(font, "Min FPS", 12);
    text_04.setPosition({90, 455});
    details.push_back(text_04);
    elements["fps_min"] = new Range({138, 453}, {30, 19}, 10, 5, 0, 60);

    elements["help_fps"] = new Button({176, 453}, {19, 19}, "?");
    elements["help_fps"]->tooltip = "Max FPS limits physics FPS. 0 means unlimited FPS\nIf FPS (top right corner) goes below Min FPS, then simulation will slow down to keep up\nIf Min FPS is set too low, simulation wont keep up and physics glitches may occur";

    sf::Text text_11(font, "Space wrapping", 12);
    text_11.setPosition({30, 475});
    details.push_back(text_11);
    elements["wrapping"] = new Checkbox({5, 473}, {19, 19}, "", true);

    elements["help_wrap"] = new Button({176, 473}, {19, 19}, "?");
    elements["help_wrap"]->tooltip = "If enabled, particles will teleport to opposite side instead of bouncing off.";

    sf::Text text_12(font, "Behaviour formula", 12);
    text_12.setPosition({5, 495});
    details.push_back(text_12);
    elements["behaviour_formula"] = new Dropdown({110, 493}, {85, 19}, {"     Particle Life", "     Newton", "     Weird"});

    sf::Text text_13(font, "Min distance", 12);
    text_13.setPosition({5, 515});
    details.push_back(text_13);
    elements["min_distance"] = new Range({78, 513}, {40, 19}, 20, 5, 0, 1000, [this]{this->elements["interaction_radius"]->value = max(this->elements["interaction_radius"]->value, this->elements["min_distance"]->value); this->elements["interaction_radius"]->update_shapes();});

    elements["help_distance"] = new Button({176, 533}, {19, 19}, "?");
    elements["help_distance"]->tooltip = "When particles are closer than Min distance, then they will repel slightly\nInteraction distance determines how far particles can attract/repel from each other\n(Uses Matrix and Behavior formula to calculate exact behavior)";

    sf::Text text_14(font, "Interation distance", 12);
    text_14.setPosition({5, 535});
    details.push_back(text_14);
    elements["interaction_radius"] = new Range({110, 533}, {40, 19}, 50, 5, 10, 1000, [this]{this->elements["min_distance"]->value = min(this->elements["min_distance"]->value, this->elements["interaction_radius"]->value); this->elements["min_distance"]->update_shapes();});

    sf::Text text_15(font, "Force multiplier", 12);
    text_15.setPosition({5, 555});
    details.push_back(text_15);
    elements["force_multiplier"] = new Range({98, 553}, {50, 19}, 2, 0.5, 0, 50);

    sf::Text text_16(font, "Use terminal velocity", 12);
    text_16.setPosition({30, 575});
    details.push_back(text_16);
    elements["use_terminal_velocity"] = new Checkbox({5, 573}, {19, 19}, "", true, [this]{this->elements["terminal_velocity"]->disable(!this->elements["use_terminal_velocity"]->value);});

    elements["help_velocity"] = new Button({176, 573}, {19, 19}, "?");
    elements["help_velocity"]->tooltip = "If terminal velocity isn't used, then particle movement will be of constant speed\nThe bigger terminal velocity value, the faster particle will slow down";

    sf::Text text_17(font, "Terminal velocity", 12);
    text_17.setPosition({5, 595});
    details.push_back(text_17);
    elements["terminal_velocity"] = new Range({104, 593}, {30, 19}, 0.9, 0.05, 0, 1);

    add_line(2, 615);

    sf::Text text_18(font, "Rendering", 14);
    text_18.setPosition({5, 620});
    text_18.setStyle(sf::Text::Bold);
    details.push_back(text_18);

    sf::Text text_19(font, "Particle vertex count", 12);
    text_19.setPosition({5, 640});
    details.push_back(text_19);
    elements["vertex_count"] = new Range({120, 638}, {30, 19}, 8, 1, 3, 32);

    sf::Text text_20(font, "Particle radius", 12);
    text_20.setPosition({5, 660});
    details.push_back(text_20);
    elements["particle_radius"] = new Range({88, 658}, {30, 19}, 2, 0.5, 0.5, 100);

    sf::Text text_21(font, "Palette", 12);
    text_21.setPosition({5, 680});
    details.push_back(text_21);
    elements["palette"] = new Dropdown({48, 678}, {95, 19}, {"     Rainbow", "     Temperature", "     Maroon", "     Pastel"}, [this]{this->matrix->used_palette = this->elements["palette"]->value; this->matrix->update_shapes();});

    sf::Text text_22(font, "Visualize velocity", 12);
    text_22.setPosition({30, 700});
    details.push_back(text_22);
    elements["visualize_velocity"] = new Checkbox({5, 698}, {19, 19}, "", false);

    add_line(3, 720);

    sf::Text text_23(font, "Info", 14);
    text_23.setPosition({5, 725});
    text_23.setStyle(sf::Text::Bold);
    details.push_back(text_23);

    elements["help_info"] = new Button({5, 745}, {75, 19}, "UI Controls");
    elements["help_info"]->tooltip = "Left Click/Scroll up - Next value / increase value\nRight Click/Scroll down - Previous value / decrease value\nMiddle Click - Reset to default value";

    elements["help_keyboard"] = new Button({5, 765}, {115, 19}, "Keyboard Controls");
    elements["help_keyboard"]->tooltip = "Space - Pause\nMiddle Mouse Button - Drag the screen\n\nScroll wheel - Zoom in/out\nArrow keys - Move around\nLeft click - Drag/Attract particles\n\nF11 - Fullscreen toggle\nCtrl - Show particle drag/brush radius\nAlt - Show spacial partitioning grid";

    elements["help_working"] = new Button({5, 785}, {70, 19}, "How to use");
    elements["help_working"]->tooltip = "Hover with cursor over boxes with ? on them for UI info\nIf bunched up particles start bouncing/exploding rapidly\nthat means time between frames is too high ,to fix it,\neither make Min FPS bigger, reduce Force multiplier or \nparticle count.\n";

    elements["help_github"] = new Button({5, 805}, {75, 19}, "Project info", []{utils::openWebPage("https://github.com/GrmSeven/CPPParticleSimulator");});
    elements["help_github"]->tooltip = "Click to open github link (MIT license)\n\nFor optimisation this project uses Spatial partitioning and Multithreading\nEverything is written in C++, visuals are rendered using SFML";
    elements["help_github"]->buttonColor = sf::Color(60, 60, 120);

    for (auto& element : elements) {
        element.second->update_shapes();
    }
}

void UserInterface::add_line(int i, int x) {
    lines[i*2 + 0].position = sf::Vector2f(0, x + 0.5);
    lines[i*2 + 1].position = sf::Vector2f(sidebar_size, x + 0.5);
    lines[i*2 + 0].color = line_color;
    lines[i*2 + 1].color = line_color;
}

void UserInterface::render(sf::RenderWindow& window) {
    window.setView(view);

    // Sidebar
    sf::RectangleShape sidebar;
    sidebar.setSize({sidebar_size, view.getSize().y});
    sidebar.setFillColor(background_color);
    sidebar.setOutlineColor(line_color);
    sidebar.setOutlineThickness(1);
    window.draw(sidebar);


    // Lines
    window.draw(lines);
    // Text
    for (auto &t : details) {
        window.draw(t);
    }
    // Elements (Buttons and stuff)
    matrix->draw(&window);
    for (auto& element : elements) {
        if (!is_element_touching(element.second, mouse_pos)) {
            element.second->draw(&window);
        }
    }
    if (get_element_at(mouse_pos) != nullptr) {
        get_element_at(mouse_pos)->draw(&window);
    }

    // FPS counter
    sf::Text text(font);

    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << fps_counter;
    text.setString(stream.str());

    text.setCharacterSize(20);

    text.setPosition({view.getSize().x - text.getLocalBounds().size.x - 10.f, 0});

    text.setStyle(sf::Text::Bold);

    window.draw(text);
}

template <typename T>
bool UserInterface::is_element_touching(Element* element, sf::Vector2<T> pos) {
    return element->contains(static_cast<sf::Vector2f>(pos));
}

Element* UserInterface::get_element_at(sf::Vector2i pos) {
    for (auto& element : elements) {
        if (is_element_touching(element.second, pos)) return element.second;
    }
    if (is_element_touching(matrix, pos)) return matrix;
    return nullptr;
}

void UserInterface::mouse_moved(sf::Vector2i pos) {
    mouse_pos = pos;
    for (auto& element : elements) {
        if (is_element_touching(element.second, pos)) {
            if (is_mouse_held) {
                Element* press_element = get_element_at(first_press_pos);
                Element* release_element = get_element_at(pos);
                if (release_element != nullptr) {
                    if (press_element == release_element) {
                        release_element->press();
                    } else {
                        release_element->hover();
                    }
                }
            } else {
                element.second->hover();
            }
        } else {
            element.second->normal();
        }
    }

    if (matrix) {
        matrix->mouse_pos = mouse_pos;
        if (is_element_touching(matrix, pos)) {
            if (is_mouse_held) {
                Element* press_element = get_element_at(first_press_pos);
                Element* release_element = get_element_at(pos);
                if (release_element != nullptr) {
                    if (press_element == release_element) {
                        release_element->press();
                    } else {
                        release_element->hover();
                    }
                }
            } else {
                matrix->hover();
            }
        } else {
            matrix->normal();
        }
    }
}


void UserInterface::mouse_pressed(sf::Vector2i pos, int type) {
    first_press_pos = pos;
    is_mouse_held = true;
    Element* press_element = get_element_at(first_press_pos);
    if (press_element != nullptr) {
        press_element->press();
    }
}

void UserInterface::mouse_released(sf::Vector2i pos, int type) {
    is_mouse_held = false;
    Element* press_element = get_element_at(first_press_pos);
    Element* release_element = get_element_at(pos);
    if (release_element != nullptr) {
        if (press_element == release_element) {
            release_element->hover();
            if (type == -1) {
                release_element->click_left();
            } else if (type == 1){
                release_element->click_right();
            } else {
                release_element->click_middle();
            }
        } else {
            release_element->hover();
        }
    }
}

void UserInterface::mouse_scrolled(sf::Vector2i pos, float scroll_delta) {
    Element* element = get_element_at(pos);
    if (element != nullptr) {
        element->hover();
        if (scroll_delta > 0) {
            element->scroll_down();
        } else {
            element->scroll_up();
        }
    }
}

void UserInterface::resize(sf::Vector2f newWindowSize) {
    view.setSize(newWindowSize);
    view.setCenter(newWindowSize / 2.f);
}
