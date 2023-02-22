#include <iostream>
#include <stddef.h>
#include "string.h"
#include "input.h"
#include "generic-field.h"
#include "ip.h"
#include "port.h"

using namespace std;


const int SUCCESS = 0;
const int FAIL = -1;
const int field_num = 2;

int main (int argc, char** argv){

    if (check_args(argc ,argv) != 0){
        return FAIL;
    }

    String *rule;

    String og_rule(argv[1]);

    size_t size = 0;

    og_rule.split(",=", &rule,&size);

    if(size != field_num){
        delete [] rule;
        return FAIL;
    }

    GenericField *field;

    // checks rule type

    if (rule[0].equals("dst-ip") || rule[0].equals("src-ip")){
        field = new Ip(rule[0]);
    }
    else if (rule[0].equals("dst-port") || rule[0].equals("src-port")){
        field = new Port(rule[0]);
    }
    else {
        delete[] rule;
        return FAIL;
    }
    
    // sets field
    if (field->set_value(rule[1]) == false){
        delete field;
        delete[] rule;
        return FAIL;
    }

    parse_input(*field);
    delete[] rule;
    delete field;

    return SUCCESS;

}
