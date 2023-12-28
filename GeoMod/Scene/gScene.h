#ifndef GSCENE_H
#define GSCENE_H

#include "../BaseTypes.h"

#include "gModel.h"

#include <vector>
#include <string>



// gScene is a class that describes any environment handled
// by GeoMod, which may or may not be displayed to the user.
// It is designed to be suitable for any application from games to 
// embedded 3D environments and all forms of CAD.

class gScene {
public:
    
    std::string guid;
    gMatrix4x4 sceneTransform;

    std::vector<gModel*> models;

};


#endif // GSCENE_H