/*///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//*/
#ifndef __FEATURE_H
#define __FEATURE_H
#include <cv.h>

/** \file Interfaces for feature detector and descriptor extractor

  I prefer these class interface functions to be non-const so that their
  results can be cached.

  The problem with this FeatureDetector version is its reliant on cv::KeyPoint.
  cv::KeyPoint is great for most cases but it doesn't allow blob-like features where
  it is better to use hull information.

  For some information about other possible implementations, please see
  http://pr.willowgarage.com/wiki/PluggableDescriptors#Requirements
*/
class FeatureDetector
{
public:
    virtual void detect(const cv::Mat& image,
                        std::vector<cv::KeyPoint>& keypoints,
                        const cv::Mat& mask = cv::Mat() ) const = 0 ;
};

/** \class DescriptorExtractor

  A virtual interface for descriptor extractors. Current implementation is gear
  toward point-based descriptor extractors. For region-based descriptor extractors,
  there is no concept of keypoints.

  \todo Consider splitting into PointDescriptorExtractor and RegionDescriptorExtractor
*/
class DescriptorExtractor
{
    /** \brief Compute sparse descriptor

      \param image Input image
      \param keypoints The interest points where to compute the descriptors
      \param[out] descriptors Output descriptors
    */
    virtual void compute(const cv::Mat& image,
                         std::vector<cv::KeyPoint>& keypoints,
                         cv::Mat& descriptors) const = 0;

    /** \brief Compute dense descriptor

      \param image Input image
      \param[out] descriptors The computed descriptors. Each line is one descriptor.
                              To access the descriptor at location (y,x). Get the descriptor
                              at row y*image.cols + x
    */
    virtual void compute_dense(const cv::Mat& image,
                         cv::Mat& descriptors);
};

/** \class DescriptorMatcher
    \brief Abstract base class for matching

    Make sure your implementation templatize for different Distance(s)
*/
class DescriptorMatcher
{
    /** \brief Index input keypoints (applicable to stuff like ANN)

      Your implementation will likely need to save the input keypoints
      and descriptors since it will likely be used again in matching.

      \param db_keypoints Input database keypoints
      \param db_descriptors Input database descriptors
      */
    virtual void index(const std::vector<cv::KeyPoint>& db_keypoints,
                       const cv::Mat& db_descriptors) = 0;

    /** \brief Find the matches in our database for the input keypoints

    \param query_keypoints Keypoints to pay attention to
    \param query_descriptors Corresponding descriptors for the keypoints
    \param matches Indices of the matches in the database keypoints
    \param distance Distance of each match
    */
    virtual void match(const std::vector<cv::KeyPoint>& query_keypoints,
                       const cv::Mat& query_descriptors,
                       std::vector<int>& matches,
                       std::vector<float>& distance) const = 0;

};

//typedef std::vector<cv::KeyPoint> KeyPointCollection;
/** \class DescriptorMatchGeneric

  A generic descriptor matcher that incorporates both extraction and matching.
  */
class DescriptorMatchGeneric
{
public:

    //! Adds keypoints from several images to the training set (descriptors are supposed to be calculated here)
    //virtual void add(KeyPointCollection& keypoints);

    //! Adds keypoints from a single image to the training set (descriptors are supposed to be calculated here)
    virtual void add(const cv::Mat& image, std::vector<cv::KeyPoint>& points) = 0;

    //! Classifies test keypoints
    virtual void classify(const cv::Mat& image, std::vector<cv::KeyPoint>& points){};

    //! Matches test keypoints to the training set
    virtual void match(const cv::Mat& image, std::vector<cv::KeyPoint>& points, std::vector<int>& indices){};

    //! Search for training keypoints in the test image
    virtual void search(const cv::Mat& test_image, std::vector<cv::KeyPoint>& output_found_points, std::vector<int>& training_point_indices){};
};



#endif
