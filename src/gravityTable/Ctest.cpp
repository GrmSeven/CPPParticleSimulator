#include <iostream>

#include "gravityElement.h"

using namespace std;

int main() {
    GravityElement *g1 = new GravityElement('1', '1');
    GravityElement *g2 = new GravityElement('2', '2');
    GravityElement *g3 = new GravityElement('3', '3');

    g1->addToElement(g2);
    g1->addToElement(g3);

    g1->printConnections();
}
