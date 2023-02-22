#ifndef IP_H
#define IP_H

#include "string.h"
#include "generic-field.h"


class Ip : public GenericField {

    unsigned int start_ip;
    unsigned int end_ip;
    String rule_field_name;
public:
//instructor definition
    Ip(String pattern);

//destructor definision
    ~Ip();

/**
* @brief check if there a match between rule and packet
* @returns True only if the value received parameter that matches the set 
* of values
*/

    bool match(String packet);

/**
* @brief Defines the set of valid values for that field
* @returns true if there were no problems with the input parser
*/
    bool set_value(String value);

};

#endif
