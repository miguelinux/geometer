#if defined(_MSC_VER) || __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 3)
#pragma once
#endif

#ifndef IL_IMAGE_H_
#define IL_IMAGE_H_

#include <fstream>
#include <cstdlib>

// this is a workaround for a bug in OpenCV:
// on Mac, if not one byte extra is allocated for the pixel memory
// most of the OpenCV functions crash (segmentation fault), such as
// cvErode, cvDilate, cvSmooth, ... (after cvCreateImageHeader and
// cvSetImageData)
#define EXTRA_BYTES_OCV		1

namespace il {

template <class T>
  class Image {
    public:
      enum channel {
      	grayscale = 1,
        rgb24     = 3
      };

      Image();

      Image(int width, int height, short ch, bool isReference = false);

      ~Image();

      T operator()(int x, int y);

      void save(const char* filename);

      void load(const char* filename);

      inline bool isReferenced() {
        return referenced;
      };

      /* pointer to aligned image data */
      union {
        T *imageData;
        T *pixels;
      };

      /* size of aligned image row in bytes */
      //int  widthStep;
      
      /* sizeof(IplImage) */
      int  size;
      /* Most of OpenCV functions support 1,2,3 or 4 channels */
      short  nChannels;
      /* pixel depth in bits: IPL_DEPTH_8U, IPL_DEPTH_8S,
       * IPL_DEPTH_16U, IPL_DEPTH_16S, IPL_DEPTH_32S,
       * IPL_DEPTH_32F and IPL_DEPTH_64F are supported
       */
      //int  depth;

      /* image width in pixels */
      int  width;
      /* image height in pixels */
      int  height;

    protected:
      bool referenced;

  };

  /***************************************************************/

  typedef Image<unsigned char>  ImageByte;
  typedef Image<double> ImageDobule;

  /***************************************************************/

  template <class T>
  Image<T>::Image() {
    width = 0;
    height = 0;
    nChannels = 0;
    referenced = false;
    imageData = 0;
    size = 0;
  };

  template <class T>
  Image<T>::Image(int width, int height,
                  short ch, bool isReference) {
    this->width = width;
    this->height = height;

    nChannels = ch;

    referenced = isReference;
    imageData = 0;
    size = 0;

    if ( ! isReference) {
      size = width*height*ch + EXTRA_BYTES_OCV;
      imageData = new T[size];
    }
  };

  template <class T>
  Image<T>::~Image() {
    if (! referenced  && imageData) {
      delete [] imageData;
    }
  };

  template <class T>
  T Image<T>::operator()(int x, int y) {
    if ( x > 0 && x <= width &&
         y > 0 && y <= height ) {
      return imageData[(y*nChannels)*width + (x*nChannels)];
    }
    return (T)0;
  }

  /*
  template <class T>
  void Image<T>::save(const char* filename) {
    std::ofstream file (filename, std::ios::out|
                                  std::ios::binary|std::ios::trunc);
    if (file.is_open()) {
      if ( ! imageData ) {
	file.close();
        return;
      }
      file.write((unsigned char*)&width,sizeof(width));
      file.write((unsigned char*)&height,sizeof(height));
      file.write((unsigned char*)&nChannels,sizeof(nChannels));

      file.write((unsigned char*)&imageData,sizeof(T)*size);
      file.close();
    }
    else {
      throw "Can not save the file";
    }
  } // save
  */

  template <class T>
  void Image<T>::save(const char* filename) {
    std::ofstream file (filename, std::ios::out|
                                  std::ios::binary|std::ios::trunc);
    if (file.is_open()) {
      if ( ! imageData ) {
	      file.close();
        return;
      }
      switch (nChannels) { 
        case grayscale:
          file << "P5\n" << width << " " << height << "\n255\n";
        break;
        case rgb24:
          file << "P6\n" << width << " " << height << "\n255\n";
        break;
      }
      file.write((const char*)imageData,sizeof(T)*(size - EXTRA_BYTES_OCV));
      file.close();
    }
    else {
      throw "Can not save the file";
    }
  } // save

  /*
  template <class T>
  void Image<T>::load(const char* filename) {
    std::ifstream file (filename, std::ios::in|std::ios::binary);
    if (file.is_open()) {
      file.read((char*)&width,sizeof(width));
      file.read((char*)&height,sizeof(height));
      file.read((char*)&nChannels,sizeof(nChannels));
      referenced = false;

      size = width*height*nChannels + EXTRA_BYTES_OCV;
      if ( ! imageData ) {
        delete [] imageData;
      }
      imageData = new T[size];
      file.read((char*)&imageData,sizeof(T)*size);
    }
    else {
      throw "Can not open the file";
    }
  } // load
  */

  template <class T>
  void Image<T>::load(const char* filename) {
    int ch, maxVal;
    char sNumber[16];
	  short idx;
    std::ifstream file (filename, std::ios::in|std::ios::binary);
    if (file.is_open()) {
      ch = file.get();
      if (ch != 'P') { file.close(); throw "Not file format header"; }
      ch = file.get();
      switch (ch) { 
        case '5':
            nChannels = grayscale;
        break;
        case '6':
            nChannels = rgb24;
        break;
        default:
            file.close(); throw "Not file format header binary";
      }
		  // Read whitespaces
      do { ch = file.get(); } while (ch == ' ' || ch == '\n' || ch == '\r');
      // Skip comments
	    while (ch == '#') { while (file.get() != '\n'); ch = file.get(); }
      // Read width
	    idx = 0;
      while (ch != ' ' && ch != '\n' && ch != '\r') {
		    sNumber[idx++] =(char) ch;
		    ch = file.get();
	    }
		  sNumber[idx] = '\0';
      width = std::atoi(sNumber);
		  // Read whitespaces
      do { ch = file.get(); } while (ch == ' ' || ch == '\n' || ch == '\r');
      // Skip comments
	    while (ch == '#') { while (file.get() != '\n'); ch = file.get(); }
      // Read height
	    idx = 0;
      while (ch != ' ' && ch != '\n' && ch != '\r') {
		    sNumber[idx++] =(char) ch;
		    ch = file.get();
	    }
		  sNumber[idx] = '\0';
      height = std::atoi(sNumber);
		  // Read whitespaces
      do { ch = file.get(); } while (ch == ' ' || ch == '\n' || ch == '\r');
      // Skip comments
	    while (ch == '#') { while (file.get() != '\n'); ch = file.get(); }
      // Read the maximum color value (Maxval)
	    idx = 0;
      while (ch != ' ' && ch != '\n' && ch != '\r') {
		    sNumber[idx++] =(char) ch;
		    ch = file.get();
	    }
		  sNumber[idx] = '\0';
      maxVal = std::atoi(sNumber);
      if (maxVal != 255) { file.close(); throw "Not file format in max Value"; }

      size = width*height*nChannels + EXTRA_BYTES_OCV;

      if ( imageData ) {
        delete [] imageData;
      }
      imageData = new T[size];
      file.read((char*)imageData, sizeof(T)*(size));
      referenced = false;
      file.close();
    }
    else {
      throw "Can not open the file";
    }
  } // load

}; // namespace il

#endif

