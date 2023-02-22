#ifndef PORT_H
#define PORT_H

#include "string.h"
#include "generic-field.h"

class Port : public GenericField{
    int max_range;
    int min_range;
    String field_type;

public:

    Port(String pattern);


    //destructor
    ~Port();


    //sets GenericField pattern to given value
    bool set_value(String value);


    //check if the packet matches the given rule for this port
    //receives packet for comparison
    //returns true if value of packet is in range, or false if not

    bool match(String packet);

};
#endif
