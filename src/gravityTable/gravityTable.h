//
// Created by root on 25.04.2025.
//

#ifndef GRAVITYTABLE_H
#define GRAVITYTABLE_H
#include <list>

#include "gravityElement.h"


struct GravityTable {
    GravityTable();

    explicit GravityTable(std::list<GravityElement*>);

    void addGravityElement(GravityElement* element);

    void removeGravityElement(GravityElement* element);

    void printConnections();

    std::list<GravityElement*> getGravityElements();

    void setGravityElements(std::list<GravityElement*> elements);

    void setForce(GravityElement* a, GravityElement* b, unsigned char force);

    private:
        std::list<GravityElement*> elements;
};



#endif //GRAVITYTABLE_H
