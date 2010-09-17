// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/* 
 * Copyright (C) 2010 RobotCub Consortium, European Commission FP6 Project IST-004370
 * Authors: Rea Francesco, Charles Clercq
 * email:   francesco.rea@iit.it, charles.clercq@iit.it
 * website: www.robotcub.org 
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */

/**
 * @file unmask.h
 * @brief code for unmasking the events
 */

#ifndef UNMASK_H
#define UNMASK_H

#include <iostream>
#include <sstream>
#include <ctime>
#include <list>

#include <yarp/os/all.h>

//Other dependency
#include <iCub/config.h>

class unmask {
private:

    /**
    * @brief This method unmasked the raw which come from the TCP socket
    * This method have been wrote by the university of zurich. contact : tobi@ini.phys.ethz.ch
    * @param *evPU A pointer on the raw casted from char* to int*
    * @param x Set with the x coordinate of the pixel
    * @param y Set with the y coordinate of the pixel
    * @param pol Set with the ON/OFF polarity of the pixel.
    * @param ts ...
    */
    void unmaskEvent(unsigned int, short&, short&, short&);



    int id;
    int nb_trame;

    int sz;
    double* buffer;
    unsigned int timestamp;
    short cartX, cartY, polarity;

    int wrapAdd;
    unsigned int xmask;
    unsigned int ymask;
    int yshift;
    int xshift;
    int polshift;
    int polmask;
    int retinalSize;
    int minValue;
    int maxValue;

    FILE* uEvents;
public:
    /**
    * @brief Function returns the pointer to the buffer that containes events counts
    * @param none
    * @return pointer to the buffer of event counts
    */
    double* getEventBuffer();

    /**
    * @brief Function that cleans buffer that containes events counts
    * @param none
    * @return none
    */
    void cleanEventBuffer();
    
    /**
    * @brief get the min number of negative events
    * @return minvalue
    */
    double getMinValue();

    /**
    * @brief get the max number of negative events
    * @return minvalue
    */
    double getMaxValue();

    //unmaskingthread(){};
    unmask();
    ~unmask();

    std::list<AER_struct> unmaskData(char*, int);

};

#endif //UNMASK_H
//----- end-of-file --- ( next line intentionally left blank ) ------------------
