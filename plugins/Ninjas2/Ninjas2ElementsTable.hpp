#ifndef NINJAS_ELEMENTS_TABLE_HPP_INCLUDED
#define NINJAS_ELEMENTS_TABLE_HPP_INCLUDED

#include <string>

// element  x , y
enum elementType {
    image,
    widget,
    box,
    label
};
struct element{
    elementType type;
    std::string name;
    int x;
    int y;
    int w;
    int h;
};

    

// images 
/* 
 * ninjas2_logo
 * clearly_broken
 */

// widgets
/*
 * load
 * polar switch
 * sensitivity 
 * slice spinner
 * slice knob
 * programs top left
 * programs offset x
 * programs offset y
 * pitchbend
 * fwd
 * rev
 * loopfwd
 * looprev
 * a
 * d
 * s
 * r
*/

// boxes
/*
 * sample
 * slice tool
 * programs
 * voice
 */

// labels
/*
 * sample
 * slice tool
 * programs
 * pitchbend
 * attack
 * decay
 * sustain
 * release
 */
