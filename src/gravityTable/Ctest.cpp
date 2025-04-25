#include <iostream>

#include "gravityElement.h"
#include "gravityTable.h"

using namespace std;

int main1() {
    GravityTable *gravityTable = new GravityTable();

    GravityElement *g1 = new GravityElement('q', '1');
    GravityElement *g2 = new GravityElement('w', '2');
    GravityElement *g3 = new GravityElement('e', '3');

    g1->addToElement(g2); //create a net
    g1->addToElement(g3);

    g2->addToElement(g1);
    g2->addToElement(g3);

    g3->addToElement(g1);
    g3->addToElement(g2);

    gravityTable->addGravityElement(g1); // add all
    gravityTable->addGravityElement(g2);
    gravityTable->addGravityElement(g3);

    gravityTable->printConnections();

    cout << endl;
    GravityTable *gravityTable2 = new GravityTable();

    gravityTable2->setGravityElements(gravityTable->getGravityElements());

    gravityTable2->printConnections();
}
