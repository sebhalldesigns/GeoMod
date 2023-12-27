#ifndef GGEOMETRY_H
#define GGEOMETRY_H

enum gGeometryType {
    SKETCH_GEOMETRY,
        // defining 2D, simple types for the purposes of 
        // parametric, constrained sketching 
        // e.g AutoCAD
    
    PARAMETRIC_GEOMETRY,
        // defining complex 3D geometry for parametric CAD,
        // supporting both B-Rep (Boundary Representation and 
        // CSG (Constructive Solid Geometry) paradigms.
        // e.g SOLIDWORKS

    DISCRETE_GEOMETRY
        // defining linear and mesh-based geometry for
        // rendering and non-parametric editing.
        // e.g Blender
};


// possible parent class for geometry 
class gGeometrySet {
public:
    gGeometryType geometryType;
};


#endif // GGEOMETRY_H