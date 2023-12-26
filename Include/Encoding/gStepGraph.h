#ifndef GSTEPGRAPH_H
#define GSTEPGRAPH_H

#include <map>
#include <string>

enum gStepEntities {
    // product info
    PRODUCT,
    PRODUCT_DEFINITION,

    // assembly info
    NEXT_ASSEMBLY_USAGE_OCCURRENCE,
    ASSEMBLY_COMPONENT_USAGE,

    // dimensional info
    DIMENSIONAL_LOCATION,
    DIMENSIONAL_SIZE,

    // text
    ANNOTATION_TEXT,
    
    // geometric point
    CARTESIAN_POINT,
    
    // geometric curves and lines
    LINE,
    CIRCLE,
    ELLIPSE,
    BSPLINE_CURVE,

    // geometric surfacees
    PLANE,
    CYLINDRICAL_SURFACE,
    SPHERICAL_SURFACE,
    TOROIDAL_SURFACE,

    // geometric solids
    MANIFOLD_SOLID_BREP,
    CSG_SOLID,

    // topological edges
    EDGE_CURVE,
    EDGE_LOOP,

    // topological vertices
    VERTEX_POINT,

    // topological faces
    FACE_SURFACE,
    FACE_BOUND,

    // material and appearance
    MATERIAL,
    COLOUR_RGB,
    SURFACE_STYLE_USAGE,
};

std::map<gStepEntities, std::string> entitiesMap = {

};

#endif // GSTEPGRAPH_H