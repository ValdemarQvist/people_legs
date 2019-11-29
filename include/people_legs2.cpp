#include "people_legs.h"

PeopleLegs::PeopleLegs() {
    detector_sub1_ = n_.subscribe<people_msgs::PositionMeasurementArray>("people_tracker_measurements_1", 1, boost::bind(&PeopleLegs::updateParamCallback_, this, _1, PeopleLegs::TRACKER_1));
    detector_sub2_ = n_.subscribe<people_msgs::PositionMeasurementArray>("people_tracker_measurements_2", 1, boost::bind(&PeopleLegs::updateParamCallback_, this, _1, PeopleLegs::TRACKER_2));
    people_pub_ = n_.advertise<people_msgs::PositionMeasurementArray>("/leg_detector", 1000);
}

bool PeopleLegs::isSamePerson_() {
    if (t_ == TRACKER_1) {
        people1_ = people_;
        distance1_.clear();
        for (const people_msgs::PositionMeasurement p : people1_) {
            ROS_INFO("detected person from %d, x: %f || y: %f", TRACKER_1, p.pos.x, p.pos.y);
        }
    }
    else if (t_ == TRACKER_2) {
        people2_ = people_;
        distance2_.clear();
	for (const people_msgs::PositionMeasurement p : people2_) {
            ROS_INFO("detected person from %d, x: %f || y: %f", TRACKER_2, p.pos.x, p.pos.y);
        }
     }
}

void PeopleLegs::updateParamCallback_(const people_msgs::PositionMeasurementArray::ConstPtr& msg, PeopleLegs::Tracker t) {
    people_ = msg->people;
    if (!people_.empty()) {
        t_ = t;
        isSamePerson_();
    }
}
