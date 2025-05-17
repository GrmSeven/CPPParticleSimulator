#include "userInterface.h"

#include <iostream>
#include <cmath>

#include "checkbox.h"
#include "range.h"
#include "dropdown.h"
#include "button.h"
#include "matrix.h"

using namespace std;

UserInterface::UserInterface(sf::Vector2f windowSize) : font("hih.ttf"), lines(sf::PrimitiveType::Lines, 2 * 5), fps_text(font)
{
    view.setSize(windowSize);
    view.setCenter(windowSize / 2.f);
    create_elements();
}

void UserInterface::create_elements() {
    elements.resize(41);

    sidebar.setSize({sidebar_size, view.getSize().y*2.0f});
    sidebar.setFillColor(background_color);
    sidebar.setOutlineColor(line_color);
    sidebar.setOutlineThickness(1);

    fps_text.setCharacterSize(20);
    fps_text.setStyle(sf::Text::Bold);
    
    sf::Text text_1(font, "Particles", 14);
    text_1.setPosition({5, 5});
    text_1.setStyle(sf::Text::Bold);
    details.push_back(text_1);

    sf::Text text_2(font, "Particle count", 12);
    text_2.setPosition({5, 25});
    details.push_back(text_2);
    elements[0] = new Range({85, 23}, {62, 19}, 1000, 100, 0, 1000000);

    sf::Text text_3(font, "Particle types", 12);
    text_3.setPosition({5, 45});
    details.push_back(text_3);
    elements[1] = new Range({85, 43}, {30, 19}, 8, 1, 1, 20, [this]{this->matrix->resize_matrix(this->elements[1]->value); this->elements[13]->max_value = this->elements[1]->value; this->elements[13]->update_shapes();});

    // Matrix
    matrix = new Matrix({5, 65}, {190, 190}, this->elements[1]->value, 0, 0.25, -1, 1);
    elements[2] = new Button({176, 45}, {19, 19}, "?");
    elements[2]->tooltip = "Matrix represents attraction force of one particle type (row) to another (column).\nEach of the types is represented by its dedicated color.\n\nFor example, if there are particles of two types nearby:\n(Type 1 is circle color to the left from a cell, Type 2 is circle color on top of a cell)\nThen Type 1 particle will get attracted or repelled to Type 2, based on the matrix cell color.\n\nBlue cell means particle will be attracted (1)\nGray cell means particle will be neutral (0)\nRed cell means particle will be repelled (-1)";


    sf::Text text_01(font, "Preset", 12);
    text_01.setPosition({5, 263});
    details.push_back(text_01);
    elements[3] = new Dropdown({45, 262}, {70, 19}, {"     Random", "      Null", "     Snakes", "     Spaghetti", "     Islands", "     Worms"});
    elements[4] = new Button({120, 262}, {40, 19}, "Apply", [this]{this->matrix->matrix_preset(this->elements[3]->value); ;});
    elements[4]->buttonColor = sf::Color(60, 60, 120);
    elements[5] = new Button({176, 262}, {19, 19}, "?");
    elements[5]->tooltip = "Choose preset for matrix and then click Apply.";

    add_line(0, 285);

    sf::Text text_4(font, "Mouse", 14);
    text_4.setPosition({5, 290});
    text_4.setStyle(sf::Text::Bold);
    details.push_back(text_4);

    sf::Text text_02(font, "Radius", 12);
    text_02.setPosition({5, 310});
    details.push_back(text_02);
    elements[6] = new Range({48, 308}, {30, 19}, 100, 5, 5, 1450);
    elements[6]->tooltip = "Radius of a brush/drag.";

    elements[35] = new Checkbox({83, 308}, {75, 19}, "     Visualize", false);
    elements[35]->tooltip = "Click to visualize brush/drag radius. It also appears while holding Ctrl.";

    sf::Text text_5(font, "Drag type", 12);
    text_5.setPosition({5, 330});
    details.push_back(text_5);
    elements[7] = new Dropdown({65, 328}, {60, 19}, {"     Attract", "     Move"});

    elements[8] = new Button({176, 328}, {19, 19}, "?");
    elements[8]->tooltip = "Use Left Mouse Button to attract/move particles within the radius\n\nAttract - Changes velocity of particle by applying a force that pulls them toward the cursor \n                  Works only when unpaused and velocity enabled\n\nMove - Changes position of particle based on distance from cursor\n                Drag force changes curvature - The bigger, the smaller curve\n\nHold Ctrl to visualize radius";

    sf::Text text_6(font, "Drag force", 12);
    text_6.setPosition({5, 350});
    details.push_back(text_6);
    elements[9] = new Range({65, 348}, {60, 19}, 25, 5, 0, 100);

    sf::Text text_7(font, "Mode", 12);
    text_7.setPosition({5, 370});
    details.push_back(text_7);
    elements[10] = new Dropdown({40, 368}, {60, 19}, {"     Spawn", "     Delete"});

    elements[11] = new Button({176, 368}, {19, 19}, "?");
    elements[11]->tooltip = "Use Right Mouse Button to Spawn/Delete particles around cursor within the radius\nSet spawn type to 0, to spawn random particles\n\nHold Ctrl to visualize radius";

    sf::Text text_8(font, "Spawn count", 12);
    text_8.setPosition({5, 390});
    details.push_back(text_8);
    elements[12] = new Range({80, 388}, {50, 19}, 50, 1, 1, 1000);

    sf::Text text_9(font, "Spawn type", 12);
    text_9.setPosition({5, 410});
    details.push_back(text_9);
    elements[13] = new Range({72, 408}, {30, 19}, 0, 1, 0, this->elements[1]->value);

    add_line(1, 430);

    sf::Text text_10(font, "Physics", 14);
    text_10.setPosition({5, 435});
    text_10.setStyle(sf::Text::Bold);
    details.push_back(text_10);

    elements[39] = new Checkbox({130, 435}, {65, 19}, "     Paused", false);

    sf::Text text_11(font, "Space wrapping", 12);
    text_11.setPosition({30, 455});
    details.push_back(text_11);
    elements[17] = new Checkbox({5, 453}, {19, 19}, "", true);
    elements[17]->tooltip = "If enabled, particles will teleport to opposite side instead of bouncing off.";

    sf::Text text_12(font, "Behaviour formula", 12);
    text_12.setPosition({5, 475});
    details.push_back(text_12);
    elements[19] = new Dropdown({110, 473}, {85, 19}, {"     Life InOut", "     Atoms", "     Life In", "     Life Out", "     Life Const"});

    sf::Text text_13(font, "Min distance", 12);
    text_13.setPosition({5, 495});
    details.push_back(text_13);
    elements[20] = new Range({78, 493}, {40, 19}, 20, 5, 0, 1000, [this]{this->elements[22]->value = max(this->elements[22]->value, this->elements[20]->value); this->elements[22]->update_shapes();});

    elements[21] = new Button({176, 493}, {19, 19}, "?");
    elements[21]->tooltip = "When particles are closer than Min distance, then they will repel slightly.\nInteraction distance determines how far particles can attract/repel from each other.\n(Uses Matrix and Behavior formula to calculate exact behavior)";

    sf::Text text_14(font, "Interaction distance", 12);
    text_14.setPosition({5, 515});
    details.push_back(text_14);
    elements[22] = new Range({110, 513}, {40, 19}, 50, 5, 10, 1000, [this]{this->elements[20]->value = min(this->elements[20]->value, this->elements[22]->value); this->elements[20]->update_shapes();});

    sf::Text text_15(font, "Force multiplier", 12);
    text_15.setPosition({5, 535});
    details.push_back(text_15);
    elements[23] = new Range({98, 533}, {30, 19}, 1, 0.2, 0, 1000);

    sf::Text text24(font, "Max velocity:", 12);
    text24.setPosition({5, 555});
    details.push_back(text24);
    elements[18] = new Range({80, 553}, {50, 19}, 1000, 100, 0, 100000);

    sf::Text text_16(font, "Use terminal velocity", 12);
    text_16.setPosition({30, 575});
    details.push_back(text_16);
    elements[24] = new Checkbox({5, 573}, {19, 19}, "", true, [this]{this->elements[26]->disable(!this->elements[24]->value);});
    elements[24]->tooltip = "If terminal velocity isn't used, then particle movement will be of constant speed\nThe bigger terminal velocity value, the faster particle will slow down";

    sf::Text text_17(font, "Terminal velocity", 12);
    text_17.setPosition({5, 595});
    details.push_back(text_17);
    elements[26] = new Range({104, 593}, {30, 19}, 0.9, 0.05, 0, 1);

    add_line(2, 615);

    sf::Text text_18(font, "Rendering", 14);
    text_18.setPosition({5, 620});
    text_18.setStyle(sf::Text::Bold);
    details.push_back(text_18);

    sf::Text text_26(font, "Visualize recursion", 12);
    text_26.setPosition({5, 640});
    details.push_back(text_26);
    elements[40] = new Dropdown({113, 638}, {54, 19}, {"     None", "     White", "     Color"});

    sf::Text text_27(font, "Draw border", 12);
    text_27.setPosition({30, 660});
    details.push_back(text_27);
    elements[25] = new Checkbox({5, 658}, {19, 19}, "", false);

    sf::Text text_19(font, "Particle vertex count", 12);
    text_19.setPosition({5, 680});
    details.push_back(text_19);
    elements[27] = new Range({120, 678}, {30, 19}, 8, 1, 3, 32);

    sf::Text text_20(font, "Particle radius", 12);
    text_20.setPosition({5, 700});
    details.push_back(text_20);
    elements[28] = new Range({88, 698}, {30, 19}, 2, 0.5, 0.5, 100);

    sf::Text text_21(font, "Palette", 12);
    text_21.setPosition({5, 720});
    details.push_back(text_21);
    elements[29] = new Dropdown({48, 718}, {95, 19}, {"     Rainbow", "     Temperature", "     Maroon", "     Pastel"}, [this]{this->matrix->used_palette = this->elements[29]->value; this->matrix->update_shapes();});

    sf::Text text_22(font, "Visualize velocity", 12);
    text_22.setPosition({5, 740});
    details.push_back(text_22);
    elements[30] = new Range({103, 738}, {40, 19}, 0, 10, 0, 5000);

    elements[38] = new Button({180, 738}, {19, 19}, "?");
    elements[38]->tooltip = "Makes fast particles brighter.\nThis changes speed threshold.\nSet to 0 to disable.";

    add_line(3, 760);

    sf::Text text_25(font, "Performance", 14);
    text_25.setPosition({5, 765});
    text_25.setStyle(sf::Text::Bold);
    details.push_back(text_25);

    sf::Text text_03(font, "Max FPS", 12);
    text_03.setPosition({5, 785});
    details.push_back(text_03);
    elements[14] = new Range({56, 783}, {30, 19}, 0, 5, 0, 500);

    sf::Text text_04(font, "Min FPS", 12);
    text_04.setPosition({90, 785});
    details.push_back(text_04);
    elements[15] = new Range({138, 783}, {30, 19}, 10, 5, 0, 60);

    elements[16] = new Button({176, 783}, {19, 19}, "?");
    elements[16]->tooltip = "Max FPS limits physics FPS. 0 means unlimited FPS.\nIf FPS (top right corner) goes below Min FPS, then simulation will slow down to keep up.\nIf Min FPS is set too low, simulation wont keep up and physics glitches may occur.";

    sf::Text text_24(font, "Partition ratio", 12);
    text_24.setPosition({5, 805});
    details.push_back(text_24);
    elements[36] = new Range({83, 803}, {30, 19}, 2, 1, 1, 10);

    sf::Text text_28(font, "Multithreading", 12);
    text_28.setPosition({30, 825});
    details.push_back(text_28);
    elements[37] = new Checkbox({5, 823}, {19, 19}, "", true);

    add_line(4, 845);

    sf::Text text_23(font, "Info", 14);
    text_23.setPosition({5, 850});
    text_23.setStyle(sf::Text::Bold);
    details.push_back(text_23);

    elements[31] = new Button({5, 870}, {75, 19}, "UI Controls");
    elements[31]->tooltip = "This applies for most UI elements:\n\nHold Shift - amplifies every action 10x times while hel.\nLeft Click/Scroll up - Next value / increase value\nRight Click/Scroll down - Previous value / decrease value\nMiddle Click - Reset to default value";

    elements[32] = new Button({5, 890}, {125, 19}, "Simulation Controls");
    elements[32]->tooltip = "Space - Pause\n\nMiddle mouse button - Drag the screen\nScroll wheel - Zoom in/out\nArrow keys - Move around\n\nShift + Scroll - Resize brush/drag\nLeft click - Drag/Attract particles\nRight click - draw/delete particles\n\nF11 - Fullscreen toggle\nCtrl - Show particle drag/brush radius\nAlt - Show spacial partitioning grid";

    elements[33] = new Button({5, 910}, {75, 19}, "How to use?");
    elements[33]->tooltip = "Hover with cursor over boxes with ? for more UI info.\n\nIf bunched up particles start bouncing/exploding rapidly\nthat means time between frames or acceleration is too high.\nTo fix it, either make Min FPS bigger, reduce Force multiplier or \nreduce particle count.";

    elements[34] = new Button({5, 930}, {75, 19}, "Project info", []{utils::openWebPage("https://github.com/GrmSeven/CPPParticleSimulator");});
    elements[34]->tooltip = "Click to open github link (MIT license)\n\nFor optimisation this project uses Spatial partitioning, Structure of Arrays and Multithreading.\nEverything is written in C++, visuals are rendered using SFML.";
    elements[34]->buttonColor = sf::Color(60, 60, 120);

    for (auto& element : elements) {
        element->update_shapes();
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
        if (!is_element_touching(element, mouse_pos)) {
            element->draw(&window);
        }
    }
    if (get_element_at(mouse_pos) != nullptr) {
        get_element_at(mouse_pos)->draw(&window);
    }

    // FPS counter
    fps_text.setString(to_string(fps_counter));
    fps_text.setPosition({view.getSize().x - fps_text.getLocalBounds().size.x - 5.f, 0});
    window.draw(fps_text);
}

template <typename T>
bool UserInterface::is_element_touching(Element* element, sf::Vector2<T> pos) {
    return element->contains(static_cast<sf::Vector2f>(pos));
}

Element* UserInterface::get_element_at(sf::Vector2i pos) {
    for (auto& element : elements) {
        if (is_element_touching(element, pos)) return element;
    }
    if (is_element_touching(matrix, pos)) return matrix;
    return nullptr;
}

void UserInterface::mouse_moved(sf::Vector2i pos) {
    mouse_pos = pos;
    for (auto& element : elements) {
        if (is_element_touching(element, pos)) {
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
                element->hover();
            }
        } else {
            element->normal();
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

void UserInterface::mouse_scrolled(sf::Vector2i pos, float scroll_delta, bool shift_pressed) {
    Element* element = get_element_at(pos);
    if (element != nullptr) {
        element->hover();
        for (int i = 0; i < (shift_pressed ? 10 : 1); i++) {
            if (scroll_delta > 0) {
                element->scroll_down();
            } else {
                element->scroll_up();
            }
        }
    }
}

void UserInterface::resize(sf::Vector2f newWindowSize) {
    view.setSize(newWindowSize);
    view.setCenter(newWindowSize / 2.f);
}
