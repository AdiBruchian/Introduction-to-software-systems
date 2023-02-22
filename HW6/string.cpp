#include <string.h>
#include <stddef.h>
#include <iostream>
#include "string.h"

#define SPACE ' '

using namespace std;



String::String() {
    length = 0;
    data = new char[length];
}




String::String(const String& str) {
    length = str.length;
    data = new char[length + 1];
    strcpy(data, str.data);
}




String::String(const char* str) {
    length = strlen(str);
    data = new char[length + 1];
    strcpy(data, str);
}



String::~String() {
    delete[] data;
}



String& String::operator=(const String& rhs) {
    if (this == &rhs) {
        return *this;
    }
    delete[] data;
    length = rhs.length;
    data = new char[length + 1];
    strcpy(data, rhs.data);
    return *this;
}



String& String::operator=(const char* str) {
    if (!(strcmp(data, str))) {
        return *this;
    }
    delete[] data;
    length = strlen(str);
    data = new char[length + 1];
    strcpy(data, str);
    return *this;
}


bool String::equals(const String& rhs) const {
    if (!(strcmp(data, rhs.data))) {
        return true;
    }
    return false;
}



bool String::equals(const char* rhs) const {
    if (!(strcmp(data, rhs))) {
        return true;
    }
    return false;
}



void  String::split(const char* delimiters, String** output, size_t* size) 
const {
    int count_portion = 0;
    char* data_cpy = new char[length + 1];
    strcpy(data_cpy, data);


    char* data_cpy2 = new char[length + 1];
    strcpy(data_cpy2, data);

    char *portion = strtok(data_cpy, delimiters);

    while (portion != NULL) {
        count_portion++;
        portion = strtok(NULL, delimiters);
    }

    *size = (size_t)count_portion;
    if (output == NULL){
        delete[] data_cpy;
        return;
    }
    *output = new String[count_portion];
    portion = strtok(data_cpy2, delimiters);
    int i = 0;
    while (portion != NULL) {
        count_portion++;
        (*output)[i] = String(portion);
        i++;
        portion = strtok(NULL, delimiters);
    }
    delete[] data_cpy;
    delete[] data_cpy2;
}


int String::to_integer() const {
    return atoi(data);
}


String String::trim() const {
    int right_edge = int(length) - 1;
    int left_edge = 0;

    while (data[right_edge] == SPACE) {
        right_edge--;
    }

    while (data[left_edge] == SPACE) {
        left_edge++;
    }

    int new_length = right_edge - left_edge + 1;
    char *new_str = new char[new_length+1];
    int current = 0;
    for (; current < new_length; left_edge++) {
        new_str[current] = data[left_edge];
        current++;
    }
    new_str[current] = '\0';
    String output = new_str;
    delete[] new_str;

    return output;
}
