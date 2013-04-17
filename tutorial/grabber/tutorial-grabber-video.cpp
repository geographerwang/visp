/*! \example tutorial-grabber-video.cpp */
#include <visp/vpDisplayX.h>
#include <visp/vpVideoReader.h>

int main()
{
#ifdef VISP_HAVE_FFMPEG
  vpImage<unsigned char> I;

  vpVideoReader g;
  g.setFileName("./video.mpg");
  g.open(I);
  g.acquire(I);
  std::cout << I.getWidth() << " " << I.getHeight() << std::endl;
  vpDisplayX d(I);
  vpDisplay::setTitle(I, "Video grabber");
  while(1) {
    g.acquire(I);
    vpDisplay::display(I);
    vpDisplay::flush(I);
    if (vpDisplay::getClick(I, false)) break;
  }
#endif
}