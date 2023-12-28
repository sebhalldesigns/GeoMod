#ifndef GMODEL_H
#define GMODEL_H

#include "../BaseTypes.h"

#include <vector>
#include <string>

// gModel is a class that describes any 3D or 2D object that
// contains geometry and physical attributes. This excludes
// entities such as lighting sources and cameras but includes
// basically everything else, from meshes and game assets to
// 2D sketches and parametric construction geometry.
//
// gModel is intended to be subclassed for specialised 
// functionalities such as parametric generation.

class gModel {
public:

    std::string guid;
    gMatrix4x4 modelTranform;

    std::vector<gModel*> children;



};

#endif // GMODEL_H