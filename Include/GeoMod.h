//  geomod, a geometric modelling kernel by seb hall
//  created on 21/06/2023
//
//  --- Features ---
//
//  geomod is a hybrid geometric kernel, designed for 
//  both solid-style modelling, (e.g mechanical CAD) as 
//  well as mesh-style modelling (e.g animation, game 
//  design). It also features a heirarchical layout
//  system, as well as advanced model loading and rendering. 
//  Intended applications include mechanical and 
//  architectural CAD software, animation design and
//  surface modelling applications. 
//

#ifndef GEOMOD_H
#define GEOMOD_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <vector>

#include "BaseTypes.h"

static const double TAU = 2.0*M_PI;

//  a point is a simple location in 3D space, with no
//  regard to size, location, direction etc
struct gPoint {
    gFloat x;
    gFloat y;
    gFloat z;

    bool IsNear(gPoint nearPoint, gFloat threshold) {
        gFloat distance = sqrt(pow(x - nearPoint.x, 2) + pow(y - nearPoint.y, 2) + pow(z - nearPoint.z, 2));
        printf("distance %f\n", distance);
        if (distance < threshold) {
            return true;
        } else {
            return false;
        }
    }
};

//  a line defines a 1-dimensional region between two points
struct gLine {
    gPoint start;
    gPoint end;
};


//  a plane can be defined as the perpendicular bisector of a line
// or simply as perpendicular to it's normal, sharing the 'start'
struct gPlane {
    gLine normal;
};

//  a circle can be defined as the perpendicular bisector of a line, 
//  with constant radius or as a plane with a normal and a constant radius.
struct gCircle {
    gPoint centre;
    gPoint radial;

    gFloat Radius() {
       return sqrt(pow(radial.x - centre.x, 2) + pow(radial.y - centre.y, 2) + pow(radial.z - centre.z, 2));
    }

    std::vector<gLine> Approximate(int lod) {
        gFloat radius = Radius();
        std::vector<gLine> workingVector;

        int numPoints = 3+(lod*3);
        for (int i = 0; i < numPoints; i++) {
            workingVector.push_back({{ centre.x + radius * sin( i * TAU / numPoints ), centre.y + radius * cos( i * TAU / numPoints ) }, { centre.x + radius * sin( (i+1)* TAU / numPoints ), centre.y + radius * cos( (i+1) * TAU / numPoints ) }});
        }

        return workingVector;
    }
};


// some other ideas


// a generic ellipse can be defined by a centre point and two radial points.
// these probably should be perpendicular but I'm not sure
struct gEllipse {
    gPoint central;
    gPoint radialA;
    gPoint radialB;
};

//  a triangle is used in mesh modelling
struct gTriangle {
    gPoint a;
    gPoint b;
    gPoint c;
};

#endif // GEOMOD_H
