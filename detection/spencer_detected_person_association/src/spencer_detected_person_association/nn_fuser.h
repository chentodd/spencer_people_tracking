/*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2014-2015, Timm Linder, Social Robotics Lab, University of Freiburg
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*
*  * Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer.
*  * Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*  * Neither the name of the copyright holder nor the names of its contributors
*    may be used to endorse or promote products derived from this software
*    without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _NN_FUSER_H
#define _NN_FUSER_H

#include <nodelet/nodelet.h>
#include "composite_detected_persons_synchronizer.h"

namespace spencer_detected_person_association
{
    /// Fuses multiple spencer_tracking_msgs/CompositeDetectedPersons messages into a joint spencer_tracking_msgs/CompositeDetectedPerson message
    /// by associating detections received on different topics based upon their distance to each other.
    /// The input messages must be in the same coordinate frame (header.frame_id), which can be ensured via ConvertToCompositeDetectionsNodelet.
    class NearestNeighborFuserNodelet : public nodelet::Nodelet, protected CompositeDetectedPersonsSynchronizer
    {
    public:
        virtual void onInit();
        virtual ~NearestNeighborFuserNodelet() {}
        NearestNeighborFuserNodelet() {}

    protected:
        /// Implements method of abstract DetectedPersonsSynchronizer base class
        virtual void onNewInputMessagesReceived(const std::vector<spencer_tracking_msgs::CompositeDetectedPersons::ConstPtr>& msgs);

        /// Compute the distance between a pair of composite detections. If outside of gating zone, returns infinity.
        virtual float computeDistance(const spencer_tracking_msgs::CompositeDetectedPerson& d1, const spencer_tracking_msgs::CompositeDetectedPerson& d2) = 0;

        /// Fuse the poses of two composite detections.
        virtual void fusePoses(const spencer_tracking_msgs::CompositeDetectedPerson& d1, const spencer_tracking_msgs::CompositeDetectedPerson& d2, geometry_msgs::PoseWithCovariance& fusedPose) = 0;

    private:
        int m_seq, m_lastDetectionId, m_detectionIdIncrement;
    };
}


#endif // _NN_FUSER_H
