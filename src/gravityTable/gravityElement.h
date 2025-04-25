#ifndef GRAVITYELEMENT_H
#define GRAVITYELEMENT_H

#include <map>

class GravityElement {
public:
    GravityElement(char name, char type);
    void addToElement(GravityElement* element);
    void printConnections() const;

private:
    char type;
    char name;
    std::map<char, std::map<char, char>> elementsNetwork; // name → {connected_name → type}
};

#endif //GRAVITYELEMENT_H
