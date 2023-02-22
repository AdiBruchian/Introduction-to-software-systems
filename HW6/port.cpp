#include "generic-field.h"
#include "port.h"
#include "string.h"

using namespace std;

const int MAX = 65535;
const int MIN = 0;


Port :: Port (String pattern) {
    max_range = 0;
    min_range = 0;
    field_type = pattern;
}

Port :: ~Port(){};


bool Port :: set_value(String value){
    String *split_string;
    size_t splits = 0;

    value.split("-", &split_string, &splits);
    if(splits != 2){
        delete[] split_string;
        return false;

    }

    min_range = split_string[0].to_integer();
    max_range = split_string[1].to_integer();


//checks range	
    if (min_range > max_range){
        min_range = 0;
        max_range = 0;
        delete[] split_string;
        return false;
    }
    if (min_range < MIN){
        min_range = 0;
        delete[] split_string;
        return false;
    }
    if (max_range > MAX){
        max_range = 0;
        delete[] split_string;
        return false;
    }

    delete[] split_string;
    return true;
}

bool Port :: match(String packet){

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
	
	if(current_field[0].equals(field_type)){
		
       		 int packet_val = current_field[1].to_integer();

       		 if((packet_val >= min_range) && (packet_val <= max_range)){
         	   	 delete[] splited_packet;
			 delete[] current_field;
			 return true;
       		 }
	}
	delete[] current_field;
    }
    
    delete[] splited_packet;
    return false;
}
