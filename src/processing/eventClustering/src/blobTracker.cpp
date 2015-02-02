#include "blobTracker.h"
using namespace cv;

BlobTracker::BlobTracker(double x0, double y0, double sig_x2, double sig_y2, double sig_xy,
                         double alpha_pos, double alpha_shape, double k, double up_thresh, double down_thresh, double delete_thresh){
    cen_x_ = x0;
    cen_y_ = y0;
    vLastX = x0;
    vLastY = y0;
    
    sig_x2_ = sig_x2;
    sig_y2_ = sig_y2;
    sig_xy_ = sig_xy;
    
    alpha_pos_ = alpha_pos;
    alpha_shape_ = alpha_shape;
    
    fixed_shape_ = false;
    
    for(int ii=0; ii<360; ii++){
        theta_.push_back(ii*M_PI/180);
    }
    
    k_ = k;
    
    up_thresh_ = up_thresh;
    down_thresh_ = down_thresh;
    delete_thresh_ = delete_thresh;
    //state_ = Inactive; // David didn't use Free
    state_ = Free;
    
    activity_ = 0;
    
    vx_ = 0;
    vy_ = 0;
    ts_last_update_ = 0;
}



BlobTracker::BlobTracker(){
}




BlobTracker::~BlobTracker() {
}



double BlobTracker::compute_p(int ev_x, int ev_y) {
    double dx = ev_x - cen_x_;
    double dy = ev_y - cen_y_;
    
    // We compute the determinant of the covariance matrix
    double det = sig_x2_*sig_y2_-sig_xy_*sig_xy_;
    
    // That we use for computing its inverse. We directly compute the resulting probability
    double tmp = (1/det)*(dx*dx*sig_y2_-2*dx*dy*sig_xy_+dy*dy*sig_x2_);
    
    return 1.0/(2*M_PI*sqrt(det))*exp(-0.5*tmp);
}



bool BlobTracker::update_activity(double temp_decay, double act){
    
    State prev_state = state_;
    activity_*=temp_decay;
    //fprintf(stdout, "[Blob TrackerTracker - update_activity] activity %f \n", activity_);
    
    // Activity threshold is a fraction of the mean/max pool activity
    double thr;
    thr = act * down_thresh_;
    //thr = down_thresh_;
    // In this stage, the activity can only go down, so it can only go from active to inactive
    if(activity_ < delete_thresh_){
        // If it drops below the delete threshold, we return true
        //fprintf(stdout, "[Blob Tracker - update_activity] set to free \n");
        state_ = Free;
        //return true;
    }
    else if(activity_ < thr){
        state_ = Inactive;
        //fprintf(stdout, "[Blob Tracker - update_activity] set to inactive \n");
    }
        //fprintf(stdout, "[Blob Tracker - update_activity] return false \n");
        //return false;

    return prev_state != state_;
}



double BlobTracker::dist2event(int ev_x, int ev_y) {
    double dx = ev_x - cen_x_;
    double dy = ev_y - cen_y_;
    
    double dist = sqrt(dx*dx+dy*dy); //compute Euclidean distance
    
    return dist;
    
}


double BlobTracker::update_position(int ev_x, int ev_y,
                                    double p, int ts, double act)
{
    activity_ += 1;//p;
    // Activity threshold is a fraction of the mean/max pool activity
    double thr;
    thr = act * up_thresh_;
    //thr = up_thresh_;
    
    if(state_==Inactive && activity_>thr){
        state_=Active;
    }
    
    double delta_sig_x2 = (ev_x-cen_x_)*(ev_x-cen_x_);
    double delta_sig_y2 = (ev_y-cen_y_)*(ev_y-cen_y_);
    double delta_sig_xy;
    
    if(fixed_shape_){
        delta_sig_x2 = (delta_sig_x2 + delta_sig_y2)/2;
        delta_sig_y2 = delta_sig_x2;
        delta_sig_xy = 0;
    }
    else{
        delta_sig_xy = (ev_x-cen_x_)*(ev_y-cen_y_);
    }
    
    sig_x2_ = (1-alpha_shape_)*sig_x2_ + alpha_shape_*delta_sig_x2;
    sig_y2_ = (1-alpha_shape_)*sig_y2_ + alpha_shape_*delta_sig_y2;
    sig_xy_ = (1-alpha_shape_)*sig_xy_ + alpha_shape_*delta_sig_xy;
    
    double old_x = cen_x_;
    double old_y = cen_y_;
    
    cen_x_ = (1-alpha_pos_)*cen_x_ + alpha_pos_*ev_x;
    cen_y_ = (1-alpha_pos_)*cen_y_ + alpha_pos_*ev_y;
    
    double alpha = 0.2;
    double dt = ts-ts_last_update_;
    
    if(dt>0){
        vx_ = (1-alpha)*vx_ + alpha*(cen_x_-old_x)/dt;
        vy_ = (1-alpha)*vy_ + alpha*(cen_y_-old_y)/dt;
    }
    ts_last_update_ = ts;
    return sqrt(std::pow(vLastX - cen_x_, 2.0) + std::pow(vLastY - cen_y_, 2.0));

}

void BlobTracker::clusterSpiked()
{
    vLastX = cen_x_;
    vLastY = cen_y_;
}



void BlobTracker::displace(double dx, double dy){
    cen_x_ += dx;
    cen_y_ += dy;
}


void BlobTracker::fix_shape(bool fix){
    fixed_shape_ = fix;
}

void BlobTracker::move_to(double x, double y){
    cen_x_ = x;
    cen_y_ = y;
}



void BlobTracker::get_ellipse_parameters(double &a, double &b, double &alpha){
    // Compute the eigenvalues of the covariance matrix

    double tmp = sqrt( (sig_x2_ - sig_y2_) * (sig_x2_ - sig_y2_) + 4*sig_xy_*sig_xy_ );
    double l_max = 0.5*(sig_x2_ + sig_y2_ + tmp);
    double l_min = 0.5*(sig_x2_ + sig_y2_ - tmp);
    
    a = sqrt(l_max);
    b = sqrt(l_min);
    alpha = 0.5*atan2f(2*sig_xy_, sig_y2_ - sig_x2_);
}

void BlobTracker::get_gauss_parameters(double &sig_x2, double &sig_y2, double &sig_xy){
    sig_x2 = sig_x2_;
    sig_y2 = sig_y2_;
    sig_xy = sig_xy_;
}


void BlobTracker::get_center(double &cen_x, double &cen_y){
    cen_x = cen_x_;
    cen_y = cen_y_;
}

void BlobTracker::getVelocity(double &v_x, double &v_y){
    v_x = vx_;
    v_y = vy_;
}

void BlobTracker::getActivity(double &act){
    act = activity_;
}

// Inactive trackers are displayed in blue, active trackers in red
void BlobTracker::display(yarp::sig::ImageOf<yarp::sig::PixelRgb> &img, yarp::sig::PixelRgb color, int id){
    
    Mat orig = (IplImage *)img.getIplImage();
    
    double a, b, alpha;
    this->get_ellipse_parameters(a, b, alpha);
    
    a*=k_;
    b*=k_;
    
    alpha = alpha * 180 / M_PI; //convert to degrees for openCV ellipse function
    
    //open CV for drawing ellipse
    
    ellipse(orig, Point(cen_y_,cen_x_), Size(a,b), alpha, 0, 360, Scalar(255,0,0));
    std::stringstream ss; ss << (int)id;
    //string str = std::to_string((int)activity_);
    putText(orig, ss.str(), Point(cen_y_,cen_x_), 1, 1, Scalar(255,0,0));

}



void BlobTracker::reset(double x0, double y0, double sig_x2, double sig_y2, double sig_xy){
    cen_x_ = x0;
    cen_y_ = y0;
    vLastX = x0;
    vLastY = y0;
    
    sig_x2_ = sig_x2;
    sig_y2_ = sig_y2;
    sig_xy_ = sig_xy;
    
    state_ = Inactive;
}	



void BlobTracker::get_norm_v(double &vx, double &vy){
    double v = sqrt(vx_*vx_ + vy_*vy_);
    vx = vx_/v;
    vy = vy_/v;
}
