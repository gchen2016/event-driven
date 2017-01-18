/*
 * Copyright (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
 * Author: Chiara Bartolozzi
 * email:  chiara.bartolozzi@iit.it
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

#ifndef __VATT__
#define __VATT__

#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <yarp/math/Math.h>
#include <iCub/emorph/all.h>
#include <iCub/emorph/vtsHelper.h>
#include <math.h>

class vAttentionManager : public yarp::os::BufferedPort<emorph::vBottle>
{
private:

    bool strictness;

    // output port for the vBottle with the new events computed by the module
    yarp::os::BufferedPort<emorph::vBottle> outPort; // /vBottle:o
    // output port where the output saliency map image (left) is sent
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelBgr> > outSalMapLeftPort;
    // output port where the output saliency map image (right) is sent
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelBgr> > outSalMapRightPort;

    int sensorSize;
    int filterSize;
    int salMapPadding;
    double tau;
    unsigned long int ptime; // past time stamp
    double thrSal; // threshold to put a maximum on the saliency map (whatever is above thr will be set to the maximum value)
    double normSal;

    emorph::vtsHelper unwrap;
    
    yarp::sig::Matrix salMapLeft;       // saliency map is a matrix (sensorSize*sensorSize)
    yarp::sig::Matrix salMapRight;      // saliency map is a matrix (sensorSize*sensorSize)
    yarp::sig::Matrix uniformFilterMap;        // filter is a matrix (filterSize*filterSize)
    yarp::sig::Matrix gaussianFilterMap;        // filter is a matrix (filterSize*filterSize)
    yarp::sig::Matrix bigGaussianFilterMap;        // filter is a matrix (filterSize*filterSize)
    yarp::sig::Matrix horizFilterMap;        // filter is a matrix (filterSize*filterSize)
    yarp::sig::Matrix vertFilterMap;        // filter is a matrix (filterSize*filterSize)
    yarp::sig::Matrix bigVertFilterMap;        // filter is a matrix (filterSize*filterSize)
    yarp::sig::Matrix bigHorizFilterMap;        // filter is a matrix (filterSize*filterSize)

    yarp::sig::Matrix vertFeatureMap;
    yarp::sig::Matrix normalisedVertFeatureMap;

    void updateMap(yarp::sig::Matrix &map, yarp::sig::Matrix &filterMap, emorph::AddressEvent *aep);
    void normaliseMap(yarp::sig::Matrix &map, yarp::sig::Matrix &normalisedMap);
    void decayMap(yarp::sig::Matrix &map, unsigned long int dt);
    void printMap(yarp::sig::Matrix &map);
    void convertToImage(yarp::sig::Matrix &map, yarp::sig::ImageOf<yarp::sig::PixelBgr> &image);
    void load_filter(std::string filename, yarp::sig::Matrix &filterMap, int &filterSize);
    double* computeAttentionPoint(yarp::sig::Matrix &map);
    void generateGaussianFilter(yarp::sig::Matrix& filterMap, double sigma, int gaussianFilterSize, int &filterSize);

public:

    vAttentionManager(int sensorSize, double tau, double thrSal, std::string &filtersPath);

    bool    open(const std::string moduleName, bool strictness = false);
    void    close();
    void    interrupt();

    //this is the entry point to your main functionality
    void    onRead(emorph::vBottle &bot);


};

class vAttentionModule : public yarp::os::RFModule
{

    //the event bottle input and output handler
    vAttentionManager      *attManager;
    yarp::os::Port          handlerPort;                 // a port to handle messages

public:

    //the virtual functions that need to be overloaded
    virtual bool configure(yarp::os::ResourceFinder &rf);
    virtual bool interruptModule();
    virtual bool close();
    virtual double getPeriod();
    virtual bool updateModule();
    bool respond(const yarp::os::Bottle& command, yarp::os::Bottle& reply);

};


#endif
//empty line to make gcc happy