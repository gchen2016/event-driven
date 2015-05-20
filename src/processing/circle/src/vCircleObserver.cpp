/*
 * Copyright (C) 2011 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
 * Author: arren.glover@iit.it
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

#include "vCircleObserver.h"
#include <vector>
#include <limits>

vCircleObserver::vCircleObserver() {

    stepbystep = false;

    width = 128;
    height = 128;
    windowSize = 200000;

    tRadius = 8;
    sRadius = 10;
    iterations = 5;
    minVsReq4RANSAC = 15;



    window = new emorph::vWindow(width, height, windowSize, false);

    inlierThreshold = 2;
}

//void vCircleObserver::pointTrim(int x, int y)
//{
//    std::vector<act_unit>::iterator i = localActivity.begin();
//    while(i != localActivity.end()) {
//        if(sqrt(std::pow(x - i->x, 2.0) + std::pow(y - i->y, 2)) < tRadius)
//            localActivity.erase(i);
//        else
//            i++;
//    }
//}

//void vCircleObserver::linearTrim(int x1, int y1, int x2, int y2)
//{
//    std::vector<act_unit>::iterator i = localActivity.begin();
//    if(x2 == x1) {
//        //if we have a vertical line just check x value
//        while(i != localActivity.end()) {
//            if(std::fabs(i->x - x1) < tRadius)
//                localActivity.erase(i);
//            else
//                i++;
//        }
//    } else {
//        //otherwise we have to calculate the line parameters
//        double m = (y2 - y1) / (double)(x2 - x1);
//        double b = y1 - m * x1;
//        while(i != localActivity.end()) {
//            if(std::fabs(i->y - m*i->x - b) < tRadius)
//                localActivity.erase(i);
//            else
//                i++;
//        }
//    }
//}

//bool vCircleObserver::localCircleEstimate(emorph::AddressEvent &event, double &cx,
//                                  double &cy, double &cr, bool showDebug)
//{
//    act_unit p1(event.getX(), event.getY(), 0);
//    //update the activity here
//    //p1.a = activity.addEvent(event);

//    //create our search locations
//    createLocalSearch(p1.x, p1.y);

//    //remove points too close to centre
//    pointTrim(p1.x, p1.y);
//    if(localActivity.size() < 3) return false;

//    //find the best score
//    int bi = 0;
//    for(int i = 0; i < localActivity.size(); i++)
//        if(localActivity[i].a > localActivity[bi].a) bi = i;
//    act_unit p2 = localActivity[bi];

//    //remove activity close-by the first point
//    linearTrim(p1.x, p1.y, p2.x, p2.y);
//    if(localActivity.empty()) return false;

//    //find the second best score
//    bi = 0;
//    for(int i = 0; i < localActivity.size(); i++)
//        if(localActivity[i].a > localActivity[bi].a) bi = i;
//    act_unit p3 = localActivity[bi];

//    //if we are all on the same y line (should be impossible after linearTrim)
//    if(p1.y == p2.y && p1.y == p3.y) return false;

//    //make sure x2 is different to x1 and x3 (else we divide by 0 later)
//    act_unit * ex = 0;

//    if(p2.x == p1.x) {
//        if(p2.x == p3.x)
//            return 0;
//        ex = &p3;
//    } else if(p2.x == p3.x) {
//        ex = &p1;
//    }

//    if(ex) {
//        act_unit temp = p2;
//        p2 = *ex;
//        *ex = temp;
//    }

//    //calculate the circle from the 3 points
//    double ma = (p2.y - p1.y) / (double)(p2.x - p1.x);
//    double mb = (p3.y - p2.y) / (double)(p3.x - p2.x);

//    cx = (ma * mb * (p1.y - p3.y) + mb * (p1.x + p2.x) -
//                        ma * (p2.x + p3.x)) / (2 * (mb - ma));
//    if(ma)
//        cy = -1 * (cx - (p1.x+p2.x)/2.0)/ma + (p1.y+p2.y)/2.0;
//    else
//        cy = -1 * (cx - (p2.x+p3.x)/2.0)/mb + (p2.y+p3.y)/2.0;

//    cr = sqrt(pow(cx - p1.x, 2.0) + pow(cy - p1.y, 2.0));



//    if(cx < 0 || cx > width-1 || cy < 0 || cy > height-1) return false;


//    if(showDebug && stepbystep) {

//        //%%% display centre and radius lines.
//        static int divider = 0;
//        if(++divider % 1 == 0) {

//            double mact = 0;
//            cv::Mat image2(128, 128, CV_32F); image2.setTo(0);
//            for(int x = 0; x < 128; x++) {
//                for(int y = 0; y < 128; y++) {
//                    //double a = activity.queryActivity(x, y);
//                    //mact = std::max(a, mact);
//                    //if(a > 0.01)
//                    //    image2.at<float>(x, y) = a;
//                }
//            }

//            image2 = image2 * (2/mact);
//            cv::Mat i8u(128, 128, CV_8U); image2.copyTo(i8u);
//            cv::Mat image(128, 128, CV_8UC3); cv::cvtColor(i8u, image, CV_GRAY2BGR);
//            cv::resize(image, image, cv::Size(512, 512), 0, 0, CV_INTER_NN);


//            cv::circle(image, cv::Point(p1.y, p1.x)*4, 12, CV_RGB(255, 255, 255));
//            cv::circle(image, cv::Point(p2.y, p2.x)*4, 12, CV_RGB(255, 255, 255));
//            cv::circle(image, cv::Point(p3.y, p3.x)*4, 12, CV_RGB(255, 255, 255));
//            cv::line(image, cv::Point(p2.y, p2.x)*4, cv::Point(p1.y, p1.x)*4, CV_RGB(255, 255, 255));
//            cv::line(image, cv::Point(p1.y, p1.x)*4, cv::Point(p3.y, p3.x)*4, CV_RGB(255, 255, 255));
//            cv::line(image, cv::Point(p1.y, p1.x)*4, cv::Point(cy, cx)*4, CV_RGB(255, 255, 255));
//            cv::line(image, cv::Point(p2.y, p2.x)*4, cv::Point(cy, cx)*4, CV_RGB(255, 255, 255));
//            cv::line(image, cv::Point(p3.y, p3.x)*4, cv::Point(cy, cx)*4, CV_RGB(255, 255, 255));
//            cv::circle(image, cv::Point(cy, cx)*4, 5, CV_RGB(255, 0, 0), CV_FILLED);



//            if(divider % 1  == 0) {
//                int xsm = std::max(p1.x-sRadius, 0)*4;
//                int ysm = std::max(p1.y-sRadius, 0)*4;
//                int wsm = std::min(128 - xsm/4, 2*sRadius)*4;
//                int hsm = std::min(128 - ysm/4, 2*sRadius)*4;

//                cv::Mat submat;
//                image(cv::Rect(ysm, xsm, hsm, wsm)).copyTo(submat);
//                cv::flip(submat, submat, 0);
//                cv::resize(submat, submat, cv::Size(0, 0), 4, 4, CV_INTER_NN);
//                cv::imshow("Local", submat);
//            }

//            cv::flip(image, image, 0);
//            cv::imshow("Activity", image);
//            char c = cv::waitKey(0);
//            if(c == 27) stepbystep = false;
//        }
//    }


//    return true;

//}

bool vCircleObserver::calculateCircle(double x1, double x2, double x3,
                                      double y1, double y2, double y3,
                                      double &cx, double &cy, double &cr)
{



    //if we are all on the same line we can't compute a circle
    if(y1 == y2 && y1 == y3) return false;
    if(x1 == x2 && x1 == x3) return false;

    //or if any of the points are duplicate
    if(x1 == x2 && y1 == y2) return false;
    if(x1 == x3 && y1 == y3) return false;
    if(x2 == x3 && y2 == y3) return false;

    //make sure x2 is different to x1 and x3 (else we divide by 0 later)
    if(x2 == x1) {
        double tx = x3, ty = y3;
        x3 = x2; y3 = y2;
        x2 = tx; y2 = ty;
    } else if(x2 == x3) {
        double tx = x1, ty = y1;
        x1 = x2; y1 = y2;
        x2 = tx; y2 = ty;
    }


    //calculate the circle from the 3 points
    double ma = (y2 - y1) / (x2 - x1);
    double mb = (y3 - y2) / (x3 - x2);

    if(ma == mb) return false;
    if(ma != ma || mb != mb) {
        std::cout << "error: (ma|mb) == NaN" << std::endl;
    }

    cx = (ma * mb * (y1 - y3) + mb * (x1 + x2) -
                        ma * (x2 + x3)) / (2 * (mb - ma));
    if(ma)
        cy = -1 * (cx - (x1+x2)/2.0)/ma + (y1+y2)/2.0;
    else
        cy = -1 * (cx - (x2+x3)/2.0)/mb + (y2+y3)/2.0;

    cr = sqrt(pow(cx - x1, 2.0) + pow(cy - y1, 2.0));
    double cr1 = cr;
    double cr2 = sqrt(pow(cx - x2, 2.0) + pow(cy - y2, 2.0));
    double cr3 = sqrt(pow(cx - x3, 2.0) + pow(cy - y3, 2.0));

    if(cx != cx) {
        std::cout << "error: cx == NaN" << std::endl;
    }
    if(cx == INFINITY || cx == -INFINITY) {
        std::cout << "error: cx == INF" << std::endl;
    }

    return true;

}

void vCircleObserver::addEvent(emorph::vEvent &event) {
    window->addEvent(event);
}

double vCircleObserver::RANSAC(double &cx, double &cy, double &cr)
{
    //emorph::vEvent *v = window->getMostRecent();
    //if(!v) return -1;
    //emorph::AddressEvent *av = v->getAs<emorph::AddressEvent>();

    emorph::vQueue q = window->getSMARTSURF(sRadius);
    q.sort();

    if(q.size() < minVsReq4RANSAC) return -1;

    int pi1, pi2, pi3;
    int max_inliers = 0;

    //this will only work for asynchronous windows
    emorph::AddressEvent *v1 = window->getMostRecent()->getAs<emorph::AddressEvent>();
    for(pi1 = 0; pi1 < q.size(); pi1++)
        if(q[pi1] == v1) break;
    //emorph::AddressEvent *v1 = q[pi1]->getAs<emorph::AddressEvent>();
    for(int i = 0; i < iterations; i++) {
        //choose three random points

        //pi1 = rand() % q.size();
        pi2 = pi1;
        while(pi2 == pi1) {
            pi2 = rand() % q.size();
        }
        pi3 = pi1;
        while(pi3 == pi1 || pi3 == pi2) {
           pi3 = rand() % q.size();
        }

        emorph::AddressEvent *v2 = q[pi2]->getAs<emorph::AddressEvent>();
        emorph::AddressEvent *v3 = q[pi3]->getAs<emorph::AddressEvent>();

        double tx, ty, tr;
        bool madeCircle = calculateCircle(v1->getX(), v2->getX(), v3->getX(),
                v1->getY(), v2->getY(), v3->getY(), tx, ty, tr);
        if(!madeCircle) continue;

        if(tr < 2*inlierThreshold) continue;
        int inliers = 0;
        for(emorph::vQueue::const_iterator qi = q.begin(); qi != q.end(); qi++) {
            emorph::AddressEvent *vp = (*qi)->getAs<emorph::AddressEvent>();
            double sqerror = fabs(pow(vp->getX() - tx, 2.0) + pow(vp->getY() - ty, 2.0)
                                 - pow(tr, 2.0));
            if(sqerror < inlierThreshold) {
                inliers++;
            }
        }

        //if(error < min_error) {
        if(inliers > max_inliers) {
            max_inliers = inliers;
            cx = tx; cy = ty; cr = tr;
        }
    }
//    bool error = false;
//    if(max_inliers >= minVsReq4RANSAC) {
//        cv::Mat a(sRadius*2+1, sRadius*2+1, CV_8UC1); a.setTo(255);
//        double cts = v1->getStamp();
//        for(emorph::vQueue::const_iterator qi = q.begin(); qi != q.end(); qi++) {
//            emorph::AddressEvent *vp = (*qi)->getAs<emorph::AddressEvent>();
//            //int x = vp->getX()-v1->getX()+sRadius;
//            //int y = vp->getY()-v1->getY()+sRadius;
//            //if(x < 0 || x > 10 || y < 0 || y > 10) error = true;
//            double tts = vp->getStamp();
//            if(tts > cts) tts = tts - emorph::vtsHelper::maxStamp();
//            if(cts - tts > 90000) tts = cts - 90000;
//            int val = 255.0 * (cts - tts)/ 100000;
//            a.at<char>(vp->getX()-v1->getX()+sRadius, vp->getY()-v1->getY()+sRadius) = val;
//        }
////        if(error) {
////            for(emorph::vQueue::const_iterator qi = q.begin(); qi != q.end(); qi++) {
////                emorph::AddressEvent *vp = (*qi)->getAs<emorph::AddressEvent>();
////                std::cout << (int)v1->getX() << " " << (int)v1->getY() << std::endl;
////                std::cout << (int)vp->getX() << " " << (int)vp->getY() << std::endl;
////            }
////            std::cout << "error reported" << std::endl;
////        }

//        //cv::Mat b(a.size()*4, CV_8UC1);
//        cv::flip(a, a, 0);
//        cv::resize(a, a, a.size()*6, 0, 0, cv::INTER_NEAREST);
//        cv::imshow("Local Surface", a);

//    }

    return max_inliers;

}

double vCircleObserver::oneShotObserve(double &cx, double &cy, double &cr)
{
    //emorph::vEvent *v = window->getMostRecent();
    //if(!v) return -1;
    //emorph::AddressEvent *av = v->getAs<emorph::AddressEvent>();

    emorph::vQueue q = window->getSMARTSURF(sRadius);
    q.sort();

    if(q.size() < minVsReq4RANSAC) return -1;

    int pi1, pi2, pi3;

    //this will only work for asynchronous windows
    emorph::AddressEvent *v1 = window->getMostRecent()->getAs<emorph::AddressEvent>();
    for(pi1 = 0; pi1 < q.size(); pi1++)
        if(q[pi1] == v1) break;

    if(pi1 == 0) {
        pi2 = q.size()-1;
        pi3 = q.size()-2;
    } else if(pi1 == 1) {
        pi2 = 0;
        pi3 = q.size()-1;
    } else {
        pi2 = pi1 - 1;
        pi3 = pi1 - 2;
    }

    emorph::AddressEvent *v2 = q[pi2]->getAs<emorph::AddressEvent>();
    emorph::AddressEvent *v3 = q[pi3]->getAs<emorph::AddressEvent>();


    bool madeCircle = calculateCircle(v1->getX(), v2->getX(), v3->getX(),
                                      v1->getY(), v2->getY(), v3->getY(),
                                      cx, cy, cr);

    if(!madeCircle) return 0;
    if(cr < 2*inlierThreshold) return 0;

    int inliers = 0;
    for(emorph::vQueue::const_iterator qi = q.begin(); qi != q.end(); qi++) {
        emorph::AddressEvent *vp = (*qi)->getAs<emorph::AddressEvent>();
        double sqerror = fabs(pow(vp->getX() - cx, 2.0) + pow(vp->getY() - cy, 2.0)
                              - pow(cr, 2.0));
        if(sqerror < inlierThreshold) {
            inliers++;
        }
    }

    return inliers;

}

void vCircleObserver::viewEvents()
{
    emorph::vQueue q = window->getSMARTSURF(sRadius);
    q.sort();

    int pi1, pi2, pi3;
    int max_inliers = 0;

    //this will only work for asynchronous windows
    emorph::AddressEvent *v1 =
            window->getMostRecent()->getAs<emorph::AddressEvent>();
    for(pi1 = 0; pi1 < q.size(); pi1++)
        if(q[pi1] == v1) break;

    if(pi1 == 0) {
        pi2 = q.size()-1;
        pi3 = q.size()-2;
    } else if(pi1 == 1) {
        pi2 = 0;
        pi3 = q.size()-1;
    } else {
        pi2 = pi1 - 1;
        pi3 = pi1 - 2;
    }

    emorph::AddressEvent *recv1 = q[pi1]->getAs<emorph::AddressEvent>();
    emorph::AddressEvent *recv2 = q[pi2]->getAs<emorph::AddressEvent>();
    emorph::AddressEvent *recv3 = q[pi3]->getAs<emorph::AddressEvent>();


    emorph::AddressEvent *ranv1 = v1, *ranv2, *ranv3;


    for(int i = 0; i < 100; i++) {

        pi2 = pi1;
        while(pi2 == pi1) {
            pi2 = rand() % q.size();
        }
        pi3 = pi1;
        while(pi3 == pi1 || pi3 == pi2) {
           pi3 = rand() % q.size();
        }

        emorph::AddressEvent *v2 = q[pi2]->getAs<emorph::AddressEvent>();
        emorph::AddressEvent *v3 = q[pi3]->getAs<emorph::AddressEvent>();

        double tx, ty, tr;
        bool madeCircle = calculateCircle(v1->getX(), v2->getX(), v3->getX(),
                v1->getY(), v2->getY(), v3->getY(), tx, ty, tr);
        if(!madeCircle) continue;

        if(tr < 2*inlierThreshold) continue;
        int inliers = 0;
        for(emorph::vQueue::const_iterator qi = q.begin(); qi != q.end(); qi++) {
            emorph::AddressEvent *vp = (*qi)->getAs<emorph::AddressEvent>();
            double sqerror = fabs(pow(vp->getX() - tx, 2.0) + pow(vp->getY() - ty, 2.0)
                                 - pow(tr, 2.0));
            if(sqerror < inlierThreshold) {
                inliers++;
            }
        }

        //if(error < min_error) {
        if(inliers > max_inliers) {
            max_inliers = inliers;
            cx = tx; cy = ty; cr = tr;
            ranv2 = v2; ranv3 = v3;
        }
    }




    cv::Mat a(sRadius*2+1, sRadius*2+1, CV_8UC1); a.setTo(255);
    double cts = v1->getStamp();
    for(emorph::vQueue::const_iterator qi = q.begin(); qi != q.end(); qi++) {
        emorph::AddressEvent *vp = (*qi)->getAs<emorph::AddressEvent>();
        //int x = vp->getX()-v1->getX()+sRadius;
        //int y = vp->getY()-v1->getY()+sRadius;
        //if(x < 0 || x > 10 || y < 0 || y > 10) error = true;
        double tts = vp->getStamp();
        if(tts > cts) tts = tts - emorph::vtsHelper::maxStamp();
        if(cts - tts > 190000) tts = cts - 190000;
        int val = 255.0 * (cts - tts)/ 200000;
        a.at<char>(vp->getX()-v1->getX()+sRadius, vp->getY()-v1->getY()+sRadius) = val;
    }
    int s = 20;
    cv::resize(a, a, a.size()*s, 0, 0, cv::INTER_NEAREST);

    cv::Mat b(a.size(), CV_8UC3);
    cv::cvtColor(a, b, CV_GRAY2BGR);
    int x, y;

    x = (recv1->getX()-v1->getX()+sRadius)*s+s/2; y = (recv1->getY()-v1->getY()+sRadius)*s+s/2;
    cv::circle(b, cv::Point(y, x), 5, CV_RGB(255, 0, 0), CV_FILLED);
    x = (recv2->getX()-v1->getX()+sRadius)*s+s/2; y = (recv2->getY()-v1->getY()+sRadius)*s+s/2;
    cv::circle(b, cv::Point(y, x), 5, CV_RGB(255, 0, 0), CV_FILLED);
    x = (recv3->getX()-v1->getX()+sRadius)*s+s/2; y = (recv3->getY()-v1->getY()+sRadius)*s+s/2;
    cv::circle(b, cv::Point(y, x), 5, CV_RGB(255, 0, 0), CV_FILLED);

    x = (ranv1->getX()-v1->getX()+sRadius)*s+s/2; y = (ranv1->getY()-v1->getY()+sRadius)*s+s/2;
    cv::circle(b, cv::Point(y, x), 3, CV_RGB(0, 255, 0), CV_FILLED);
    x = (ranv2->getX()-v1->getX()+sRadius)*s+s/2; y = (ranv2->getY()-v1->getY()+sRadius)*s+s/2;
    cv::circle(b, cv::Point(y, x), 3, CV_RGB(0, 255, 0), CV_FILLED);
    x = (ranv3->getX()-v1->getX()+sRadius)*s+s/2; y = (ranv3->getY()-v1->getY()+sRadius)*s+s/2;
    cv::circle(b, cv::Point(y, x), 3, CV_RGB(0, 255, 0), CV_FILLED);



    cv::flip(b, b, 0);
    cv::imshow("Local Surface", b);
}

int vCircleObserver::gradientView()
{

    int MIN4GRAD = 5;
    emorph::vQueue test1 = window->getSMARTSURF(1);
    if(test1.size() < MIN4GRAD) return -1;

    std::vector<double> ms, bs;
    emorph::vQueue q = window->getSMARTSURF(sRadius);
    emorph::AddressEvent *vr = window->getMostRecent()->getAs<emorph::AddressEvent>();

    int scale = 20;
    double x1 = 0, x2 = sRadius*2+1;
    double cts = vr->getStamp();
    cv::Mat a(sRadius*2+1, sRadius*2+1, CV_8UC1); a.setTo(255);
    //plot the surface
    for(emorph::vQueue::const_iterator qi = q.begin(); qi != q.end(); qi++) {
        emorph::AddressEvent *vp = (*qi)->getAs<emorph::AddressEvent>();
        double tts = vp->getStamp();
        if(tts > cts) tts = tts - emorph::vtsHelper::maxStamp();
        if(cts - tts > 190000) tts = cts - 190000;
        int val = 255.0 * (cts - tts)/ 200000;
        a.at<char>(vp->getX()-vr->getX()+sRadius, vp->getY()-vr->getY()+sRadius) = val;
    }
    cv::Mat colour(a.size(), CV_8UC3);
    cv::cvtColor(a, colour, CV_GRAY2BGR);
    cv::resize(colour, colour, colour.size()*scale, 0, 0, cv::INTER_NEAREST);
//    cv::imshow("Local Gradients", colour);
//    cvWaitKey(10);



    for(emorph::vQueue::iterator qi = q.begin(); qi != q.end(); qi++) {
        emorph::AddressEvent * v = (*qi)->getAs<emorph::AddressEvent>();
        emorph::vQueue g = window->getSURF(v->getX(), v->getY(), 1, v->getPolarity());
        if(g.size() < MIN4GRAD) continue;
        double dtdx = 0, dtdy = 0;
        int cntx = 0, cnty = 0;
        for(int i = 0; i < g.size(); i++)  {
            for(int j = i+1; j < g.size(); j++) {
                emorph::AddressEvent * v1 = g[i]->getAs<emorph::AddressEvent>();
                emorph::AddressEvent * v2 = g[j]->getAs<emorph::AddressEvent>();
                if(v1->getY() == v2->getY()) {
                    dtdx += (double)(v1->getStamp() - v2->getStamp()) / (double)(v1->getX() - v2->getX());
                    cntx++;
                }
                if(v1->getX() == v2->getX()) {
                    dtdy += (double)(v1->getStamp() - v2->getStamp()) / (double)(v1->getY() - v2->getY());
                    cnty++;
                }

            }
        }
        if(cntx == 0 || cnty == 0) continue;
        dtdx /= cntx;
        dtdy /= cnty;
        double m = dtdy / dtdx; //atan2(dxdt, dydt);
        double b = v->getY() - m * v->getX();
        b = (v->getY() - vr->getY() + sRadius) -m * (v->getX() - vr->getX() + sRadius);

        if(v == vr) {
            ms.insert(ms.begin(), 1, m);
            bs.insert(bs.begin(), 1, b);
        } else {
            ms.push_back(m);
            bs.push_back(b);
        }

        double y1 = (ms.back() * x1 + bs.back());
        double y2 = (ms.back() * x2 + bs.back());
        //cv::Mat wrec = colour.clone();
        cv::line(colour, cv::Point((y1)*scale, (x1)*scale), cv::Point((y2)*scale, (x2)*scale), CV_RGB(255, 0, 0), 0.5);


        //cv::Rect rw(cv::Point((tfy)*scale, (tfx)*scale), cv::Size(3*scale, 3*scale));
        //cv::rectangle(wrec, rw, CV_RGB(0, 0, 255));
        //cv::imshow("Local Gradients", colour);
        //cv::waitKey(100);
        //std::cout << "dtdx: " << dtdx << "dtdy: " << dtdy << " m: " << m << " b: " << b << std::endl;
        //cv::waitKey();

    }

    if(ms.size() < 3) return -1;

    double cx, cy, cr_scaled;
    int inliers = 0, max_inliers = 0;

    int pi1, pi2;
    for(int i = 0; i < 100; i++) {
        pi1 = 0; //rand() % ms.size();

        pi2 = pi1;
        while(pi2 == pi1)
                pi2 = rand() % ms.size();

        if(ms[pi1] == ms[pi2]) continue;
        double xc = (bs[pi2] - bs[pi1]) / (ms[pi1] - ms[pi2]);
        double yc = (ms[pi1]*bs[pi2] - ms[pi2]*bs[pi1]) / (ms[pi1] - ms[pi2]);

        inliers = 0;
        for(int j = 0; j < ms.size(); j++) {
            double d = fabs(-ms[j]*xc + yc - bs[j]) / sqrt(pow(ms[j], 2.0) + 1);
            if(d < 0.5)
                inliers++;
        }

        if(inliers > max_inliers) {
            max_inliers = inliers;
            cx = xc;
            cy = yc;

        }



    }


    cr_scaled = sqrt(pow(cx*scale - (sRadius+0.5)*scale, 2.0) + pow(cy*scale - (sRadius+0.5)*scale, 2.0));
    std::cout << max_inliers << std::endl;
    cv::circle(colour, cv::Point(cy*scale, cx*scale), 0.5*scale, CV_RGB(0, 255, 0), 0.5);
    cv::circle(colour, cv::Point(cy*scale, cx*scale), cr_scaled, CV_RGB(0, 0, 255), 0.5);


    cv::flip(colour, colour, 0);

    cv::imshow("Local Gradients", colour);

    return max_inliers;

}

int vCircleObserver::gradientView2()
{

    int MIN4GRAD = 5;
    int scale = 10;

    std::vector<double> ms, bs;
    double main_m, main_b;
    emorph::vQueue test1 = window->getSMARTSURF(1);
    //if(test1.size() < MIN4GRAD) return -1;

    emorph::vQueue q = window->getSMARTSURF(sRadius);
    emorph::AddressEvent *vr = window->getMostRecent()->getAs<emorph::AddressEvent>();


    double cts = vr->getStamp();
    cv::Mat a(128, 128, CV_8UC1); a.setTo(255);
    //plot the surface
    for(emorph::vQueue::const_iterator qi = q.begin(); qi != q.end(); qi++) {
        emorph::AddressEvent *vp = (*qi)->getAs<emorph::AddressEvent>();
        double tts = vp->getStamp();
        if(tts > cts) tts = tts - emorph::vtsHelper::maxStamp();
        if(cts - tts > 190000) tts = cts - 190000;
        int val = 255.0 * (cts - tts)/ 200000;
        a.at<char>(vp->getX(), vp->getY()) = val;
    }
    cv::Mat colour(a.size(), CV_8UC3);
    cv::cvtColor(a, colour, CV_GRAY2BGR);
    //cv::resize(colour, colour, colour.size(), 0, 0, cv::INTER_NEAREST);
//    cv::imshow("Local Gradients", colour);
//    cvWaitKey(10);



    for(emorph::vQueue::iterator qi = q.begin(); qi != q.end(); qi++) {
        emorph::AddressEvent * v = (*qi)->getAs<emorph::AddressEvent>();
        emorph::vQueue g = window->getSURF(v->getX(), v->getY(), 1, v->getPolarity());
        if(g.size() < MIN4GRAD) continue;
        double dtdx = 0, dtdy = 0;
        int cntx = 0, cnty = 0;
        for(int i = 0; i < g.size(); i++)  {
            for(int j = i+1; j < g.size(); j++) {
                emorph::AddressEvent * v1 = g[i]->getAs<emorph::AddressEvent>();
                emorph::AddressEvent * v2 = g[j]->getAs<emorph::AddressEvent>();
                if(v1->getY() == v2->getY()) {
                    dtdx += (double)(v1->getStamp() - v2->getStamp()) / (double)(v1->getX() - v2->getX());
                    cntx++;
                }
                if(v1->getX() == v2->getX()) {
                    dtdy += (double)(v1->getStamp() - v2->getStamp()) / (double)(v1->getY() - v2->getY());
                    cnty++;
                }

            }
        }
        if(cntx == 0 || cnty == 0) continue;
        dtdx /= cntx;
        dtdy /= cnty;

        double m = dtdy / dtdx; //atan2(dxdt, dydt);
        double b = v->getY() - m * v->getX();
        //b = (v->getY() - vr->getY() + sRadius) -m * (v->getX() - vr->getX() + sRadius);


        if(v == vr) {
            main_m = m;
            main_b = b;
            //cv::line(colour, cv::Point(y1, x1), cv::Point(y2, x2), CV_RGB(255, 0, 255), 0.1);
        } else {
            ms.push_back(m);
            bs.push_back(b);
            //cv::line(colour, cv::Point(y1, x1), cv::Point(y2, x2), CV_RGB(255, 0, 0), 0.1);
        }

    }

    if(ms.size() < 1) return -1;

    double meanx = 0, meany = 0;
    std::vector<double> cxs, cys;
    double mincx = vr->getX()-sRadius, maxcx = vr->getX()+sRadius;
    double mincy = vr->getY()-sRadius, maxcy = vr->getY()+sRadius;
    for(int i = 0; i < ms.size(); i++) {
        double xc = (bs[i] - main_b) / (main_m - ms[i]);
        double yc = (main_m*bs[i] - ms[i]*main_b) / (main_m - ms[i]);
        cxs.push_back(xc);
        cys.push_back(yc);
        meanx += xc; meany += yc;
        mincx = std::min(mincx, xc);
        maxcx = std::max(maxcx, xc);
        mincy = std::min(mincy, yc);
        maxcy = std::max(maxcy, yc);
    }
    mincx = std::max(mincx, 0.0); maxcx = std::min(maxcx, 127.0);
    mincy = std::max(mincy, 0.0); maxcy = std::min(maxcy, 127.0);
    meanx /= ms.size(); meany /= ms.size();

    //cv::rectangle(colour, cv::Rect(mincy, mincx, maxcy-mincy+1, maxcx-mincx+1), CV_RGB(0, 0, 0));
    cv::Mat subcolour = colour(cv::Rect(mincy, mincx, maxcy-mincy+1, maxcx-mincx+1));
    cv::Mat m3;
    cv::resize(subcolour, m3, subcolour.size()*scale, 0, 0, cv::INTER_NEAREST);

    double x1 = 0; double x2 = (maxcx-mincx+1);
    double y1 = main_m * (x1+mincx) + main_b - mincy;
    double y2 = main_m * (x2+mincx) + main_b - mincy;
    cv::line(m3, cv::Point(y1*scale, x1*scale), cv::Point(y2*scale, x2*scale), CV_RGB(255, 0, 255), 0.1);
    for(int i = 0; i < ms.size(); i++) {

        y1 = ms[i] * (x1+mincx) + bs[i] - mincy;
        y2 = ms[i] * (x2+mincx) + bs[i] - mincy;

        cv::circle(m3, cv::Point((cys[i]-mincy)*scale, (cxs[i]-mincx)*scale), 3, CV_RGB(0, 0, 255), CV_FILLED);
        cv::line(m3, cv::Point(y1*scale, x1*scale), cv::Point(y2*scale, x2*scale), CV_RGB(255, 0, 0), 0.1);

    }
     cv::circle(m3, cv::Point((meany-mincy)*scale, (meanx-mincx)*scale), 4, CV_RGB(255, 0, 255), CV_FILLED);






    cv::flip(m3, m3, 0);
    //cv::resize(colour, colour, colour.size()*4);

    cv::imshow("Local Gradients 2", m3);

    return 0;

}

int vCircleObserver::gradient(double &cx, double &cy, double &cr)
{

    int MIN4GRAD = 6;
    emorph::vQueue test1 = window->getSMARTSURF(1);
    if(test1.size() < MIN4GRAD) return -1;

    std::vector<double> ms, bs;
    emorph::vQueue q = window->getSMARTSURF(sRadius);
    emorph::AddressEvent *vr = window->getMostRecent()->getAs<emorph::AddressEvent>();

    for(emorph::vQueue::iterator qi = q.begin(); qi != q.end(); qi++) {
        emorph::AddressEvent * v = (*qi)->getAs<emorph::AddressEvent>();
        emorph::vQueue g = window->getSURF(v->getX(), v->getY(), 1, v->getPolarity());
        if(g.size() < MIN4GRAD) continue;
        double dtdx = 0, dtdy = 0;
        int cntx = 0, cnty = 0;
        for(int i = 0; i < g.size(); i++)  {
            for(int j = i+1; j < g.size(); j++) {
                emorph::AddressEvent * v1 = g[i]->getAs<emorph::AddressEvent>();
                emorph::AddressEvent * v2 = g[j]->getAs<emorph::AddressEvent>();
                if(v1->getY() == v2->getY()) {
                    dtdx += (double)(v1->getStamp() - v2->getStamp()) / (double)(v1->getX() - v2->getX());
                    cntx++;
                }
                if(v1->getX() == v2->getX()) {
                    dtdy += (double)(v1->getStamp() - v2->getStamp()) / (double)(v1->getY() - v2->getY());
                    cnty++;
                }

            }
        }
        if(cntx == 0 || cnty == 0) continue;
        dtdx /= cntx;
        dtdy /= cnty;
        double m = dtdy / dtdx; //atan2(dxdt, dydt);   
        double b = v->getY() - m * v->getX();

        if(v == vr) {
            ms.insert(ms.begin(), 1, m);
            bs.insert(bs.begin(), 1, b);
        } else {
            ms.push_back(m);
            bs.push_back(b);
        }


    }

    if(ms.size() < 3) return -1;

    int inliers = 0, max_inliers = 0;

    int pi1, pi2;
    for(int i = 0; i < 100; i++) {
        pi1 = 0; //rand() % ms.size();

        pi2 = pi1;
        while(pi2 == pi1)
                pi2 = rand() % ms.size();

        if(ms[pi1] == ms[pi2]) continue;
        double xc = (bs[pi2] - bs[pi1]) / (ms[pi1] - ms[pi2]);
        double yc = (ms[pi1]*bs[pi2] - ms[pi2]*bs[pi1]) / (ms[pi1] - ms[pi2]);

        inliers = 0;
        for(int j = 0; j < ms.size(); j++) {
            double d = fabs(-ms[j]*xc + yc - bs[j]) / sqrt(pow(ms[j], 2.0) + 1);
            if(d < 0.5)
                inliers++;
        }

        if(inliers > max_inliers) {
            max_inliers = inliers;
            cx = xc;
            cy = yc;

        }



    }

    cr = sqrt(pow(cx - vr->getX(), 2.0) + pow(cy - vr->getY(), 2.0));
    if(cr < 5) return -1;
    return max_inliers;

}

double vCircleObserver::globalInlierCount(double cx, double cy, double cr)
{
    const emorph::vQueue q = window->getSTW(cx, cy, cr+inlierThreshold);
    int inliers = 0;
    for(emorph::vQueue::const_iterator qi = q.begin(); qi != q.end(); qi++) {
        emorph::AddressEvent *v = (*qi)->getAs<emorph::AddressEvent>();
        double terror = fabs(pow(v->getX() - cx, 2.0) + pow(v->getY() - cy, 2.0)
                             - pow(cr, 2.0));
        if(sqrt(terror) < inlierThreshold) {
            inliers++;
        }
    }
    return inliers;
}

/*//////////////////////////////////////////////////////////////////////////////
  CIRCLE TRACKER
  ////////////////////////////////////////////////////////////////////////////*/

vCircleTracker::vCircleTracker(double svPos, double svSiz, double zvPos, double zvSiz)
{
    this->svPos = svPos;
    this->svSiz = svSiz;
    active = false;
    pTS = 0;

    //these two matrices are dependent on dt so we have to update them everytime
    yarp::sig::Matrix A(9, 9); A = 0;
    A(0, 0) = 1; A(1, 1) = 1; A(2, 2) = 1;
    A(3, 3) = 1; A(4, 4) = 1; A(5, 5) = 1;
    A(6, 6) = 1; A(7, 7) = 1; A(8, 8) = 1;
    //we need to update A: (0, 3), (1, 4) and (2, 5) based on delta t

    yarp::sig::Matrix Q(9, 9); Q = 0;
    //we update Q depending on delta t

    //these two are constant
    yarp::sig::Matrix H(9, 9); H = 0;
    H(0, 0) = 1; H(1, 1) = 1; H(2, 2) = 1;

    yarp::sig::Matrix R(9, 9); R = 0;
    R(0, 0) = zvPos; R(1, 1) = zvPos; R(2, 2) = zvSiz;

    filter = new iCub::ctrl::Kalman(A, H, Q, R);

}

vCircleTracker::~vCircleTracker()
{
    if(filter) delete filter;
}

//add event will:
//1. check closeness to distribution
//2. update zq if < c
//3. perform correction based on observation from zq
//4. destroy if validation gate too high
//5. return whether, updated, non-updated, needs destroying
double vCircleTracker::addEvent(emorph::AddressEvent v, double cT)
{

    //get our delta t
    double ts = unwrap(v.getStamp());
    double dt = 0;
    if(!pTS) {
        pTS = ts;
    } else {
        dt = (ts - pTS) * unwrap.tstosecs();
        pTS = ts;
    }

    //predict
    predict(dt);
    yarp::sig::Vector state = filter->get_x();
    if(state[0] != state[0]) {
        std::cout << "error" << std::endl;
    }

    //assume it is part of the tracker (probability = 1)
    double cV = 1;
    if(active) {
        //calculate probability observation belongs to this tracker
        cV = Pvgd(v.getX(), v.getY());

    } else {
        //some other measure whether this belongs to a circle otherwise we
        //would make circles from any three random points

    }

    std::cout << cV << " <--> " << cT << std::endl;
    if(cV < cT) {
        return 0;
    }



    //either it's not active or the z fits the distribution
    zq.push_front(v.clone());

    //still not active
    if(zq.size() < 3)
        return 0;

    //or we throw out old observations
    if(zq.size() > 3) {
        delete zq.back();
        zq.pop_back();
    }


    //calculate the circle
    double cx, cy, cr;
    bool madecircle = makeObservation(cx, cy, cr);


    //if we didn't make a circle just exit?
    if(!madecircle)
        return 0;

    //adding observation
    std::cout << "NEW OBSERVATION" << std::endl;

    //either correct our position or start tracking
    yarp::sig::Vector z(6, 0.0); z[0]=cx; z[1]=cy; z[2]=cr;
    if(active) {
        filter->correct(z);
        yarp::sig::Vector state = filter->get_x();
        if(state[0] != state[0]) {
            std::cout << "error" << std::endl;
        }
    } else {
        yarp::sig::Matrix P0 = filter->get_R();
        P0(3, 3) = P0(0, 0); P0(4, 4) = P0(1, 1); P0(5, 5) = P0(2, 2);
        filter->init(z, P0);
        active = true;
    }

    //return our tracking strength
    //this might not be a good measure if we only add observations that agree
    //with the distribution in the first place
    return filter->get_ValidationGate();

}

double vCircleTracker::correct(double xz, double yz, double rz, double dt)
{
    if(!active) return 0;
    yarp::sig::Vector z(9, 0.0); z[0]=xz; z[1]=yz; z[2]=rz;
    filter->correct(z);
    return 0;
}

double vCircleTracker::init(double xz, double yz, double rz)
{
    yarp::sig::Vector x0(9, 0.0); x0[0] = xz; x0[1] = yz; x0[2] = rz;
    yarp::sig::Matrix P0 = filter->get_R();
    P0(3, 3) = P0(0, 0); P0(4, 4) = P0(1, 1); P0(5, 5) = P0(2, 2);
    P0(4, 4) = P0(0, 0); P0(5, 5) = P0(1, 1); P0(6, 6) = P0(2, 2);
    filter->init(x0, P0);
    active = true;
    return 0;
}

//update state
//also update zq values based on velocity
double vCircleTracker::predict(double dt)
{

    if(!active) return 0;
    //update the tracker position
    yarp::sig::Matrix A = filter->get_A();
    double dt2 = 0.5 * pow(dt, 2.0);
    A(0, 3) = dt; A(1, 4) = dt; A(2, 5) = dt;
    A(3, 6) = dt; A(4, 7) = dt; A(5, 8) = dt;
    A(0, 6) = dt2; A(1, 7) = dt2; A(2, 8) = dt2;
    filter->set_A(A);

    yarp::sig::Matrix Q = filter->get_Q();
    Q(6, 6) = svPos*dt2; Q(7, 7) = svPos*dt2; Q(8, 8) = svSiz*dt2;
    filter->set_Q(Q);

    filter->predict();

    //update the observations according to tracker movement
//    yarp::sig::Vector x = filter->get_x();
//    emorph::vQueue::iterator vi;
//    for(vi = zq.begin(); vi != zq.end(); vi++) {
//        emorph::AddressEvent *v = (*vi)->getAs<emorph::AddressEvent>();
//        if(!v) continue;
//        v->setX(v->getX() + x(0) * dt);
//        v->setY(v->getY() + x(1) * dt);
//    }


    return 0;
}

double vCircleTracker::Pvgd(double xv, double yv)
{
    if(!active) return 0;

    yarp::sig::Vector c = filter->get_x();
    double xc = c[0], yc = c[1], rc = c[2];

    //calculate the distance to the mean of the distribution
    double rv = sqrt(pow(yv-yc, 2.0) + pow(xv-xc, 2.0));
    if(rv == 0) return 0;
    double xd = xc + (rc / rv) * (xv - xc); //trouble here if rv == 0
    double yd = yc + (rc / rv) * (yv - yc);

    double x = fabs(xv - xd);
    double y = fabs(yv - yd);

    yarp::sig::Matrix P = filter->get_P();
    double distance = P(0, 0) * pow(x, 2.0) + 2 * P(0, 1) * x * y +
            P(1, 1) * pow(y, 2.0);
    distance = pow(x, 2.0) + pow(y, 2.0);
    double det = pow(P(0, 0), 2.0) + pow(P(1, 1), 2.0) + 2 * P(0, 0) * P(1, 1) *
            P(0, 1) + pow(P(2, 2), 2.0);

    double pvgd = exp(-0.5 * distance / det);
    return pvgd;// / sqrt(det * 6.283);
}

double vCircleTracker::Pzgd(double xz, double yz, double rz)
{
    if(!active) return 0;

    yarp::sig::Vector x = filter->get_x();
    yarp::sig::Matrix P = filter->get_P();

    double Px = exp(-0.5 * pow(xz - x[0], 2.0) / pow(P(0, 0), 2.0));
    double Py = exp(-0.5 * pow(yz - x[1], 2.0) / pow(P(1, 1), 2.0));
    double Pr = exp(-0.5 * pow(rz - x[2], 2.0) / pow(P(2, 2), 2.0));

    return std::min(Px, std::min(Py, Pr));

}

bool vCircleTracker::makeObservation(double &cx, double &cy, double &cr)
{
    //extract the points
    if(zq.size() < 3) return false;
    emorph::AddressEvent *v1 = zq[0]->getAs<emorph::AddressEvent>();
    emorph::AddressEvent *v2 = zq[1]->getAs<emorph::AddressEvent>();
    emorph::AddressEvent *v3 = zq[2]->getAs<emorph::AddressEvent>();
    double x1 = v1->getX(), y1 = v1->getY();
    double x2 = v2->getX(), y2 = v2->getY();
    double x3 = v3->getX(), y3 = v3->getY();

    //if we are all on the same line we can't compute a circle
    if(y1 == y2 && y1 == y3) return false;
    if(x1 == x2 && x1 == x3) return false;
    if(x1 == x2 && y1 == y2) return false;
    if(x1 == x3 && y1 == y3) return false;
    if(x2 == x3 && y2 == y3) return false;

    //make sure x2 is different to x1 and x3 (else we divide by 0 later)
    if(x2 == x1) {
        x3 = x2; y3 = x2;
        x2 = v3->getX(); y2 = v3->getY();
    } else if(x2 == x3) {
        x1 = x2; y1 = y2;
        x2 = v1->getX(); y2 = v1->getY();
    }


    //calculate the circle from the 3 points
    double ma = (y2 - y1) / (x2 - x1);
    double mb = (y3 - y2) / (x3 - x2);

    if(ma == mb) return false;
    if(ma != ma || mb != mb) {
        std::cout << "error" << std::endl;
    }

    cx = (ma * mb * (y1 - y3) + mb * (x1 + x2) -
                        ma * (x2 + x3)) / (2 * (mb - ma));
    if(ma)
        cy = -1 * (cx - (x1+x2)/2.0)/ma + (y1+y2)/2.0;
    else
        cy = -1 * (cx - (x2+x3)/2.0)/mb + (y2+y3)/2.0;

    cr = sqrt(pow(cx - x1, 2.0) + pow(cy - y1, 2.0));

    if(cx != cx) {
        std::cout << "error" << std::endl;
    }
    if(cx == INFINITY || cx == -INFINITY) {
        std::cout << "error" << std::endl;
    }

    return true;

}


