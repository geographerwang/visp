/****************************************************************************
 *
 * $Id: testMeEllipse1.cpp,v 1.6 2006-09-08 15:52:33 fspindle Exp $
 *
 * Copyright (C) 1998-2006 Inria. All rights reserved.
 *
 * This software was developed at:
 * IRISA/INRIA Rennes
 * Projet Lagadic
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * http://www.irisa.fr/lagadic
 *
 * This file is part of the ViSP toolkit
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE included in the packaging of this file.
 *
 * Licensees holding valid ViSP Professional Edition licenses may
 * use this file in accordance with the ViSP Commercial License
 * Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact visp@irisa.fr if any conditions of this licensing are
 * not clear to you.
 *
 * Description:
 * Tracking of an ellipse.
 *
 * Authors:
 * Eric Marchand
 * Fabien Spindler
 *
 *****************************************************************************/

/*!
  \example testMeCircle1.cpp

  \brief Tracking of an ellipse.
*/

#include <stdio.h>
#include <sstream>
#include <iomanip>

#include <visp/vpDebug.h>
#include <visp/vpConfig.h>

#if (defined (VISP_HAVE_X11) || defined(VISP_HAVE_GTK) || defined(WIN32))

#include <visp/vpImage.h>
#include <visp/vpImageIo.h>
#include <visp/vpDisplayX.h>
#include <visp/vpDisplayGTK.h>
#include <visp/vpDisplayGDI.h>
#include <visp/vpColor.h>

#include <visp/vpMeEllipse.h>
#include <visp/vpParseArgv.h>
#include <visp/vpIoTools.h>

// List of allowed command line options
#define GETOPTARGS	"ci:h"

/*!

  Print the program options.

  \param ipath: Input image path.

*/
void usage(char *name, char *badparam, string ipath)
{
  fprintf(stdout, "\n\
Test auto detection of dots using vpDot2.\n\
\n\
SYNOPSIS\n\
  %s [-i <input image path>] [-c] [-h]\n", name);

  fprintf(stdout, "\n\
OPTIONS:                                               Default\n\
  -i <input image path>                                %s\n\
     Set image input path.\n\
     From this path read images \n\
     \"ViSP-images/ellipse-1/image.%%04d.pgm\"\n\
     Setting the VISP_INPUT_IMAGE_PATH environment\n\
     variable produces the same behaviour than using\n\
     this option.\n\
\n\
  -c\n\
     Disable the mouse click. Usefull to automaze the \n\
     execution of this program without humain intervention.\n\
\n\
  -d \n\
     Turn off the display.\n\
\n\
  -h\n\
     Print the help.\n",
	  ipath.c_str());

}
/*!

  Set the program options.

  \param ipath : Input image path.
  \return false if the program has to be stopped, true otherwise.

*/
bool getOptions(int argc, char **argv, string &ipath, bool &click_allowed, bool &display)
{
  char *optarg;
  int	c;
  while ((c = vpParseArgv::parse(argc, argv, GETOPTARGS, &optarg)) > 1) {

    switch (c) {
    case 'c': click_allowed = false; break;
    case 'd': display = false; break;
    case 'i': ipath = optarg; break;
    case 'h': usage(argv[0], NULL, ipath); return false; break;

    default:
      usage(argv[0], optarg, ipath);
      return false; break;
    }
  }

  if ((c == 1) || (c == -1)) {
    // standalone param or error
    usage(argv[0], NULL, ipath);
    cerr << "ERROR: " << endl;
    cerr << "  Bad argument " << optarg << endl << endl;
    return false;
  }

  return true;
}


int
main(int argc, char ** argv)
{
  string env_ipath;
  string opt_ipath;
  string ipath;
  string dirname;
  string filename;
  bool opt_click_allowed = true;
  bool opt_display = true;

  // Get the VISP_IMAGE_PATH environment variable value
  char *ptenv = getenv("VISP_INPUT_IMAGE_PATH");
  if (ptenv != NULL)
    env_ipath = ptenv;

  // Set the default input path
  if (! env_ipath.empty())
    ipath = env_ipath;


  // Read the command line options
  if (getOptions(argc, argv, opt_ipath, opt_click_allowed, opt_display) == false) {
    exit (-1);
  }

  // Get the option values
  if (!opt_ipath.empty())
    ipath = opt_ipath;

  // Compare ipath and env_ipath. If they differ, we take into account
  // the input path comming from the command line option
  if (opt_ipath.empty()) {
    if (ipath != env_ipath) {
      cout << endl
	   << "WARNING: " << endl;
      cout << "  Since -i <visp image path=" << ipath << "> "
	   << "  is different from VISP_IMAGE_PATH=" << env_ipath << endl
	   << "  we skip the environment variable." << endl;
    }
  }

  // Test if an input path is set
  if (opt_ipath.empty() && env_ipath.empty()){
    usage(argv[0], NULL, ipath);
    cerr << endl
	 << "ERROR:" << endl;
    cerr << "  Use -i <visp image path> option or set VISP_INPUT_IMAGE_PATH "
	 << endl
	 << "  environment variable to specify the location of the " << endl
	 << "  image path where test images are located." << endl << endl;
    exit(-1);
  }


  // Declare an image, this is a gray level image (unsigned char)
  // it size is not defined yet, it will be defined when the image will
  // read on the disk
  vpImage<unsigned char> I ;

  // Set the path location of the image sequence
  dirname = ipath +  vpIoTools::path("/ViSP-images/ellipse-1/");

  // Build the name of the image file
  unsigned iter = 1; // Image number
  std::ostringstream s;
  s.setf(ios::right, ios::adjustfield);
  s << "image." << std::setw(4) << std::setfill('0') << iter << ".pgm";
  filename = dirname + s.str();

  // Read the PGM image named "filename" on the disk, and put the
  // bitmap into the image structure I.  I is initialized to the
  // correct size
  //
  // exception readPGM may throw various exception if, for example,
  // the file does not exist, or if the memory cannot be allocated
  try{
    vpCTRACE << "Load: " << filename << endl;

    vpImageIo::readPGM(I, filename) ;
  }
  catch(...)
  {
    // an exception is throwned if an exception from readPGM has been catched
    // here this will result in the end of the program
    // Note that another error message has been printed from readPGM
    // to give more information about the error
    cerr << endl
	 << "ERROR:" << endl;
    cerr << "  Cannot read " << filename << endl;
    cerr << "  Check your -i " << ipath << " option " << endl
	 << "  or VISP_INPUT_IMAGE_PATH environment variable."
	 << endl;
    exit(-1);
  }

  // We open a window using either X11, GTK or GDI.
#if defined VISP_HAVE_X11
  vpDisplayX display;
#elif defined VISP_HAVE_GTK
  vpDisplayGTK display;
#elif defined WIN32
  vpDisplayGDI display;
#endif

  if (opt_display) {
    try{
      // Display size is automatically defined by the image (I) size
      display.init(I, 100, 100,"Display...") ;
      // Display the image
      // The image class has a member that specify a pointer toward
      // the display that has been initialized in the display declaration
      // therefore is is no longuer necessary to make a reference to the
      // display variable.
      vpDisplay::display(I) ;
    }
    catch(...)
    {
      vpERROR_TRACE("Error while displaying the image") ;
      exit(-1);
    }
  }

  vpMeEllipse E1 ;

  vpMe me ;
  me.setRange(20) ;
  me.setSampleStep(2) ;
  me.setPointsToTrack(60) ;
  me.setThreshold(15000) ;


  E1.setMe(&me) ;
  E1.setDisplay(vpMeTracker::RANGE_RESULT) ;
  E1.initTracking(I) ;
  if (opt_display) {
    E1.display(I, vpColor::green) ;
  }

  vpERROR_TRACE("sample step %f ",E1.me->sample_step) ;
  E1.track(I) ;
  if (opt_display && opt_click_allowed) {
    cout << "A click to continue..." << endl;
    vpDisplay::getClick(I) ;
  }
  cout <<"------------------------------------------------------------"<<endl;


  for (int iter = 1 ; iter < 1500 ; iter++)
  {
    // set the new image name
    s.str("");
    s << "image." << std::setw(4) << std::setfill('0') << iter << ".pgm";
    filename = dirname + s.str();
    cout << "Tracking on image: " << filename << endl;
    // read the image
    vpImageIo::readPGM(I, filename);
    if (opt_display) {
      // Display the image
      vpDisplay::display(I) ;
    }
    try
    {
      E1.track(I) ;
    }
    catch(...)
    {
      vpERROR_TRACE("Error in tracking vpMeLine ") ;
      exit(1) ;
    }

    if (opt_display) {
      E1.display(I,vpColor::green) ;
    vpDisplay::flush(I) ;
    }
  }
  if (opt_display && opt_click_allowed) {
    cout << "A click to exit..." << endl;
    vpDisplay::getClick(I) ;
  }
}
#else
int
main()
{
  vpERROR_TRACE("You do not have X11, GTK or GDI display functionalities...");
}

#endif
