#ifndef GRAVITYELEMENT_H
#define GRAVITYELEMENT_H

#include <map>

class GravityElement {
public:
    GravityElement(char name, float type);
    void addToElement(GravityElement* element, float force);
    void printConnections() const;
    void removeGravityElement(GravityElement* element);

    void setForceTo(GravityElement* element, float force);


private:
    char type;
    char name;
    std::map<char, std::map<char, float>> elementsNetwork; // name → {connected_name → type}
};

#endif //GRAVITYELEMENT_H
