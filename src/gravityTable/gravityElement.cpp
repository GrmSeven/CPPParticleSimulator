//
// Created by root on 25.04.2025.
//

#include "gravityElement.h"


#include <iostream>

GravityElement::GravityElement(char name, unsigned char type) : type(type), name(name) {
    elementsNetwork[name][name] = type;
}

void GravityElement::addToElement(GravityElement* element, unsigned char force) {
    elementsNetwork[name][element->name] = force;
}

void GravityElement::printConnections() const {
    for (const auto& [from, connections] : elementsNetwork) {
        std::cout << "From " << from << ": ";
        for (const auto& [to, type] : connections) {
            std::cout << "(" << to << ", " << type << ") ";
        }
        std::cout << std::endl;
    }
}

void GravityElement::removeGravityElement(GravityElement *element) {
    elementsNetwork.erase(element->name);
}

void GravityElement::setForceTo(GravityElement* element, unsigned char force) {
    elementsNetwork[name][element->name] = force;
}
