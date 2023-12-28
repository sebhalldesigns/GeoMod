#ifndef GSCENESYSTEM_H
#define GSCENESYSTEM_H

#include "../BaseTypes.h"

// gSceneSystem is a base class for any scene modifying system
// such as physics, particle systems, online dependent or 
// constraint-based CAD layouts.

enum gSceneSystemTrigger {
    // no automatic triggering
    SELF_TRIGGER,
    
    // framerate-based triggering
    EVERY_FRAME_TRIGGER,
    EVERY_OTHER_FRAME_TRIGGER,

    // framerate-independent triggering
    CRITICAL_PRIORITY_TRIGGER, // nominally 120 Hz
    HIGH_PRIORITY_TRIGGER, // nominally 60 Hz
    MEDIUM_PRIORITY_TRIGGER, // nominally 30 Hz
    LOW_PRIORITY_TRIGGER, // nominally 10 Hz
    LOWER_PRIORITY_TRIGGER, // nominally 1 Hz
    VERY_LOW_PRIORITY_TRIGGER, // nominally 0.1 Hz (every 10s)
    LOWEST_PRIORITY_TRIGGER, // nominally 0.01 Hz (every 100s)
};

class gSceneSystem {
public:
    gSceneSystemTrigger trigger;

    virtual void Update(gDuration timeSinceLastUpdate) {
        
    }
};

#endif // GSCENESYSTEM_H