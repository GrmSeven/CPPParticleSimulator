#include "gravityElement.h"


#include <chrono>
#include <iostream>
#include <random>

GravityElement::GravityElement(char name, float type) : type(type), name(name) {
    elementsNetwork[name][name] = type;
}

void GravityElement::addToElement(GravityElement* element, float force) {
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

void GravityElement::randomizeNet() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_real_distribution<> dis(-1.0, 1.0); //standart code to create a random number between -1f and 1f

    //        from -> groupName connections-> map<secondGroupName, Force>
    for (auto& [from, connections] : elementsNetwork) { //start to iterate all connections inside of element
        for (auto& [to, type] : connections) { //get all stenches
             type = dis(gen); // force random
        }
    }
}

void GravityElement::removeGravityElement(GravityElement *element) {
    elementsNetwork.erase(element->name);
}

void GravityElement::setForceTo(GravityElement* element, float force) {
    elementsNetwork[name][element->name] = force;
}

char GravityElement::getName() const {
    return name;
}

std::map<char, float> GravityElement::getForcesFrom() const {
    return this->elementsNetwork.at(name);
}
