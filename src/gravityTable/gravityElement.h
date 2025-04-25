#ifndef GRAVITYELEMENT_H
#define GRAVITYELEMENT_H

#include <map>

class GravityElement {
public:
    GravityElement(char name, unsigned char type);
    void addToElement(GravityElement* element, unsigned char force);
    void printConnections() const;
    void removeGravityElement(GravityElement* element);

    void setForceTo(GravityElement* element, unsigned char force);


private:
    char type;
    char name;
    std::map<char, std::map<char, char>> elementsNetwork; // name → {connected_name → type}
};

#endif //GRAVITYELEMENT_H
