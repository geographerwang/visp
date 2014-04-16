/****************************************************************************
 *
 * $Id: vpMbtDistanceCircle.h 4649 2014-02-07 14:57:11Z fspindle $
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2014 by INRIA. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 *
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Description:
 * Manage a circle used in the model-based tracker.
 *
 * Authors:
 * Fabien Spindler
 *
 *****************************************************************************/

/*!
 \file vpMbtDistanceCircle.h
 \brief Manage a circle used in the model-based tracker.
*/

#ifndef vpMbtDistanceCircle_HH
#define vpMbtDistanceCircle_HH

#include <visp/vpPoint.h>
#include <visp/vpMeEllipse.h>
#include <visp/vpHomogeneousMatrix.h>
#include <visp/vpFeatureEllipse.h>
#include <visp/vpCircle.h>

/*!
  \class vpMbtDistanceCircle
  
  \brief Manage a circle used in the model-based tracker.

  \ingroup ModelBasedTracking
*/
class VISP_EXPORT vpMbtDistanceCircle
{
  private :
    std::string name;
    unsigned int index;
    vpCameraParameters cam;
    vpMe *me;
    //double alpha;
    double wmean;
    vpFeatureEllipse featureEllipse ;
    
  public: 
    //! The moving edge containers
    vpMeEllipse *meEllipse;

    //! The circle to track
    vpCircle *circle;

    //! The radius of the circle
    double radius;

    //! The center of the circle
    vpPoint *p1;
    //! A point on the plane containing the circle
    vpPoint *p2;
    //! An other point on the plane containing the circle
    vpPoint *p3;

    //! The interaction matrix
    vpMatrix L;
    //! The error vector
    vpColVector error;
    //! The number of moving edges
    unsigned int nbFeature;
    //! Indicates if the circle has to be reinitialized
    bool Reinit;
    
  public:
    vpMbtDistanceCircle() ;
    ~vpMbtDistanceCircle() ;

    void buildFrom(const vpPoint &_p1, const vpPoint &_p2, const vpPoint &_p3, const double r);
    
    void computeInteractionMatrixError(const vpHomogeneousMatrix &cMo);
    
    void display(const vpImage<unsigned char> &I, const vpHomogeneousMatrix &cMo, const vpCameraParameters &cam, const vpColor col, const unsigned int thickness = 1);
    void display(const vpImage<vpRGBa> &I, const vpHomogeneousMatrix &cMo, const vpCameraParameters &cam, const vpColor col, const unsigned int thickness = 1);
    void displayMovingEdges(const vpImage<unsigned char> &I);
    
    /*!
     Get the camera paramters.
   
     \param camera : The vpCameraParameters used to store the camera parameters.
    */
    inline void getCameraParameters(vpCameraParameters& camera) {camera = this->cam;}
    
    /*!
      Get the index of the circle.
      
      \return Return the index of the line.
    */
    inline unsigned int getIndex() {return index ;}
    
    /*!
     Get the mean weight of the circle. The mean weight is computed thanks to the weight of each moving edge.
     Those weights are computed by the robust estimation method used during the virtual visual servoing.
   
     \return The mean weight of the circle.
    */
    inline double getMeanWeight() const {return wmean;}
        
    /*!
      Get the name of the circle.
      
      \return Return the name of the circle.
    */
    inline std::string getName() const {return name;}
    
    void initInteractionMatrixError();
    
    void initMovingEdge(const vpImage<unsigned char> &I, const vpHomogeneousMatrix &cMo);
    
    void reinitMovingEdge(const vpImage<unsigned char> &I, const vpHomogeneousMatrix &cMo);
    
    /*!
     Set the camera paramters.
     \param camera : The camera parameters.
    */
    inline void setCameraParameters(const vpCameraParameters& camera) {this->cam = camera;}
    
    /*!
      Set the index of the circle.
      
      \param i : The index number
    */
    inline void setIndex(const unsigned int i) {index = i;}

    /*!
     Set the mean weight of the circle.

     \param _wmean : The mean weight of the circle.
    */
    inline void setMeanWeight(const double _wmean) {this->wmean = _wmean;}
    
    void setMovingEdge(vpMe *Me);
    
    /*!
      Set the name of the circle.
      
      \param circle_name : The name of the circle.
    */
    inline void setName(const std::string& circle_name) {this->name = circle_name;}
    
    /*!
      Set the name of the circle.
      
      \param circle_name : The name of the circle.
    */
    inline void setName(const char* circle_name) {this->name = std::string(circle_name);}

    void trackMovingEdge(const vpImage<unsigned char> &I, const vpHomogeneousMatrix &cMo);
    
    void updateMovingEdge(const vpImage<unsigned char> &I, const vpHomogeneousMatrix &cMo);

  private:
    void project(const vpHomogeneousMatrix &cMo);
} ;

#endif
