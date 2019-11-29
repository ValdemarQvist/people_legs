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
            distance1_.emplace_back(sqrt(square(p.pos.x) + square(p.pos.y)));
            for (int i = 0; i < distance2_.size(); i++) {
                if (fabs(distance1_.back() - distance2_.at(i)) < 0.01) {
                    detected_people_.emplace_back(p);
                }
            }
        }
    }
    else if (t_ == TRACKER_2) {
        people2_ = people_;
        distance2_.clear();
        for (const people_msgs::PositionMeasurement p : people2_) {
            distance2_.emplace_back(sqrt(square(p.pos.x) + square(p.pos.y)));
            for (int i = 0; i < distance1_.size(); i++) {
                if (fabs(distance2_.back() - distance1_.at(i)) < 0.01) {
                    detected_people_.emplace_back(p);
                }
            }
        }
    }
    if (detected_people_.size()) {
        ROS_INFO("people detected");
        return true;
    }
}

void PeopleLegs::updateParamCallback_(const people_msgs::PositionMeasurementArray::ConstPtr& msg, PeopleLegs::Tracker t) {
    people_ = msg->people;
    if (!people_.empty()) {
        t_ = t;
        if (isSamePerson_()) {
	        int i = 0;
            for (const people_msgs::PositionMeasurement p : detected_people_) {
                ROS_INFO("person %d detected, x %f || y : %f", ++i, p.pos.x, p.pos.y);
            }
            detected_people_.clear();
        }
    }
}