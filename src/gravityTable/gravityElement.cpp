//
// Created by root on 25.04.2025.
//

#include "gravityElement.h"


#include <iostream>

GravityElement::GravityElement(char name, char type) : name(name), type(type) {
    elementsNetwork[name][name] = type;
}

void GravityElement::addToElement(GravityElement* element) {
    elementsNetwork[name][element->name] = element->type;
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