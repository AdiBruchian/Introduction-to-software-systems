#include "string.h"
#include "generic-field.h"
#include "ip.h"

#define MIN_IP 0
#define MAX_IP 0xFFFFFFFF

#define ONE_BYTE 8
#define TWO_BYTE 16
#define THREE_BYTE 24
#define BITS_INT 32


Ip :: Ip(String pattern) {
    start_ip = 0;
    end_ip = 0;
    rule_field_name = pattern;

}

Ip :: ~Ip() {}

bool Ip :: match(String packet) {
    // Separates the 4 different fields of the packet, according to "," sign
    String *splited_packet;
    size_t packet_parts_num;
    packet.split(",", &splited_packet, &packet_parts_num);

    // Separates each field into the field name and field value, according to 
    //"=" sign
    for (size_t i = 0; i < packet_parts_num; ++i) {
        String *current_field;
        size_t fields_num;
        splited_packet[i].split("=", &current_field, &fields_num);

        //trim removes the spaces
        current_field[0] = current_field[0].trim(); //holds the field name
        current_field[1] = current_field[1].trim(); //holds the field value

        //divide the ip address into it's 4 parts, according the '.' sign
        String* splited_dot;
        size_t splited_dot_num;
        current_field[1].split(".", &splited_dot, &splited_dot_num);

        //representation of the IP address as a single 32-bit integer value
        if (rule_field_name.equals(current_field[0])) {
            unsigned int ip = ((splited_dot[0].trim().to_integer()<<THREE_BYTE)
                             | (splited_dot[1].trim().to_integer() << TWO_BYTE)
                             | (splited_dot[2].trim().to_integer() << ONE_BYTE)
                             | (splited_dot[3].trim().to_integer()));


            if ((ip >= start_ip) && (ip <= end_ip)) {
		    delete[] splited_packet;
         	    delete[] splited_dot;
           	    delete[] current_field;
		    return true;
            }
           // else return false;
        }
	delete[] splited_dot;
	delete[] current_field;
    }
    delete[] splited_packet;
    return false;
}

bool Ip :: set_value(String value) {

    //divide the rule value into two parts: 1.ip 2.mask, according the '/' sign
    String *splited_slash;
    size_t split_slash_size;
    value.split("/", &splited_slash, &split_slash_size);
    int mask_bits = splited_slash[1].trim().to_integer();

    if (mask_bits < 0 || mask_bits > BITS_INT || split_slash_size == 0) {
        delete[] splited_slash;
        return false;
    }

    //divide the ip address into it's 4 parts, according the '.' sign
    String *splited_dot;
    size_t splited_dot_num;
    splited_slash[0].split(".", &splited_dot, &splited_dot_num);

    //representation of the IP address as a single 32-bit integer value
    unsigned int ip = ( (splited_dot[0].trim().to_integer() << THREE_BYTE)
                        | (splited_dot[1].trim().to_integer() << TWO_BYTE)
                        | (splited_dot[2].trim().to_integer() << ONE_BYTE)
                        | (splited_dot[3].trim().to_integer()) );

    unsigned int rule_mask = 0;
    if (mask_bits != 0) {
        rule_mask = MAX_IP << (BITS_INT - mask_bits);
    }

    start_ip = ip & rule_mask;
    end_ip = ip | (~rule_mask);

   // delete[] splited_rule;
    delete[] splited_slash;
    delete[] splited_dot;
    return true;
}
