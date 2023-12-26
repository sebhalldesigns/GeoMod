#ifndef GSKETCHGEOMETRY_H
#define GSKETCHGEOMETRY_H
#include "../BaseTypes.h"
#include <vector>
#include <memory>

struct gSketchPoint {
    gFloat x;
    gFloat y;
};

struct gSketchOffset {
    gFloat x;
    gFloat y;
};

class gSketchLine {
public:
    
    std::shared_ptr<gSketchPoint> start;
    std::shared_ptr<gSketchPoint> end;

};

// used to depict circles and arcs
class gSketchArc {
public:
    
    std::shared_ptr<gSketchPoint> centre;
    gFloat radius;
    gFloat angleStart;
    gFloat angleDuration;

};


enum gSketchContstraintType {
    
    // single point constraints
    ABSOLUTE_CONSTRAINT, // absolute location - Point, offset
    RELATIVE_CONSTRAINT, // location derived from another point or an external reference
    
    // line constraints
    ANGLE, // line at an angle, e.g horizontal, vertical
    TANGENT, // tangent constraint is just a linear location with an absolute offset, references an arc

    // multiple circle or point constraints 
    CONCENTRIC, 



    COINCIDENT_CONSTRAINT,
    
};

struct gSketchConstraint {
    gSketchContstraintType type;
    gSketchOffset offset;
};


class gSketchSet {
public:
    std::vector<std::shared_ptr<gSketchPoint>> points;

};


#endif //GSKETCHGEOMETRY_H