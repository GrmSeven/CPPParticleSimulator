//
// Created by root on 25.04.2025.
//

#ifndef GRAVITYTABLE_H
#define GRAVITYTABLE_H
#include <list>

#include "gravityElement.h"


/**
 * mini DOC
 * if you need to create table -> use a standard constructor without parameters
 * if you need to create a new table from existed -> use a contractor with table as parameter
 * to remove element from network -< use a special method and give as parameter element for deleting
 *
 * also you can, get all elements in network, print all network, set force between 2-groups, set random force for all groups.
 */

struct GravityTable {
    GravityTable();

    explicit GravityTable(std::list<GravityElement*>);

    void addGravityElement(GravityElement* element);

    void removeGravityElement(GravityElement* element);

    void printConnections();

    std::list<GravityElement*> getGravityElements();

    void setGravityElements(std::list<GravityElement*> elements);

    void setForce(GravityElement* a, GravityElement* b, float force);

    void randomize() const; //inside this method you can look, how to work with map

    private:
        std::list<GravityElement*> elements;
};



#endif //GRAVITYTABLE_H
