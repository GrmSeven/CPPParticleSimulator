#include <iostream>

#include "gravityElement.h"
#include "gravityTable.h"

using namespace std;

int main1() {
    GravityTable *gravityTable = new GravityTable();

    GravityElement *g1 = new GravityElement('a', 0);
    GravityElement *g2 = new GravityElement('b', .3);
    GravityElement *g3 = new GravityElement('c', 1);

    g1->addToElement(g2, 1); //create a net
    g1->addToElement(g3,0);

    g2->addToElement(g1,0);
    g2->addToElement(g3,0);

    g3->addToElement(g1,0);
    g3->addToElement(g2,1);

    gravityTable->addGravityElement(g1); // add all
    gravityTable->addGravityElement(g2);
    gravityTable->addGravityElement(g3);

    gravityTable->printConnections();

    cout << endl;
    GravityTable *gravityTable2 = new GravityTable();

    gravityTable2->setForce(g1, g2, 9);

    gravityTable2->setGravityElements(gravityTable->getGravityElements());

    gravityTable2->printConnections();



}
