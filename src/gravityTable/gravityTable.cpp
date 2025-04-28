//
// Created by root on 25.04.2025.
//

#include "gravityTable.h"

#include <iostream>

GravityTable::GravityTable() = default;

GravityTable::GravityTable(std::list<GravityElement *> elements): elements(elements) {
}

void GravityTable::addGravityElement(GravityElement *element) {
    elements.push_back(element);
}

void GravityTable::removeGravityElement(GravityElement *element) {
    elements.remove(element);
}

void GravityTable::printConnections() {
    for (GravityElement* element : elements) {
        element->printConnections();
    }
}

std::list<GravityElement *> GravityTable::getGravityElements() {
    return elements;
}

void GravityTable::setGravityElements(std::list<GravityElement *> elements) {
    for (auto* element : elements) {
        if (element) {
            this->elements.push_back(new GravityElement(*element));
        }
    }
}

void GravityTable::setForce(GravityElement* a, GravityElement* b, float force) {
    //не Ньютоновская логика
    a->setForceTo(b, force);
}

void GravityTable::randomize() const { //we start to get all elements from Net
    for (const auto &element : this->elements) {
        element->randomizeNet(); // we start to randomize each element in list
    }
}

GravityElement *GravityTable::getGravityElement(char gropName) const {
    for (auto *gravElement : this->elements) {
        if (gravElement->getName() == gropName) {
            return gravElement;
        }
    }
    return nullptr; //idk will be dangerous
};
