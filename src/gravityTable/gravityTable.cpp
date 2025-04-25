//
// Created by root on 25.04.2025.
//

#include "gravityTable.h"

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
    this->elements = std::move(elements);
}

void GravityTable::setForce(GravityElement* a, GravityElement* b, unsigned char force) {
    //не Нь.тоновкая логика
    a->setForceTo(b, force);
}
