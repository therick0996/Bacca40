
#pragma once

#include "quantum.h"

#define ____ KC_NO


// This a shortcut to help you visually see your layout.
// The first section contains all of the arguments
// The second converts the arguments into a two-dimensional array

#define LAYOUT_ortho_4x12x1( \
    k001, k002, k003, k004, k005, k006, k007, k008, k009, k010, k011, k012,   k013, \
    k101, k102, k103, k104, k105, k106, k107, k108, k109, k110, k111, k112,   k113, \
    k201, k202, k203, k204, k205, k206, k207, k208, k209, k210, k211, k212,   k213, \
    k301, k302, k303, k304, k305, k306, k307, k308, k309, k310, k311, k312,   k313  \
) \
{ \
    { k001, k002, k003, k004, k005, k006, k007, k008, k009, k010, k011, k012,   k013, }, \
    { k101, k102, k103, k104, k105, k106, k107, k108, k109, k110, k111, k112,   k113, }, \
    { k201, k202, k203, k204, k205, k206, k207, k208, k209, k210, k211, k212,   k213, }, \
    { k301, k302, k303, k304, k305, k306, k307, k308, k309, k310, k311, k312,   k313  }  \
}

#define LAYOUT_ortho_4x12( \
    k001, k002, k003, k004, k005, k006, k007, k008, k009, k010, k011, k012,   k013, \
    k101, k102, k103, k104, k105, k106, k107, k108, k109, k110, k111, k112,   k113, \
    k201, k202, k203, k204, k205, k206, k207, k208, k209, k210, k211, k212,   k213, \
    k301, k302, k303, k304, k305, k306, k307, k308, k309, k310, k311, k312,   k313  \
) \
{ \
    { k001, k002, k003, k004, k005, k006, k007, k008, k009, k010, k011, k012,   ____, }, \
    { k101, k102, k103, k104, k105, k106, k107, k108, k109, k110, k111, k112,   ____, }, \
    { k201, k202, k203, k204, k205, k206, k207, k208, k209, k210, k211, k212,   ____, }, \
    { k301, k302, k303, k304, k305, k306, k307, k308, k309, k310, k311, k312,   ____  }  \
}
