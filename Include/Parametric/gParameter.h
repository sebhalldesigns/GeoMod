#ifndef GPARAMETER_H
#define GPARAMETER_H

#include "../BaseTypes.h"
#include <string>
#include <map>

enum gParameterType {
    GFLOAT,
    GINT,
    STRING,

    GSIZE,
    GOFFSET,
};

union gParameterData {
    gFloat gfloatData;
    gInt gintData;
    std::string stringData;
    gSize gsizeData;
    gOffset goffsetData; 
};


struct gParameter {
    std::string parameterName;
    gParameterType type;
    gParameterData data;
};

class gParameterSet {
    std::map<std::string, gParameter> parameterSet; 
public:

    

};

#endif // GPARAMETER_H