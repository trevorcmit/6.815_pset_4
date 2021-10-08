// hdr.cpp
// Assignment 5

#include "hdr.h"
#include "filtering.h"
#include <algorithm>
#include <math.h>

using namespace std;

/**************************************************************
 //                       HDR MERGING                        //
 *************************************************************/

Image computeWeight(const Image &im, float epsilonMini, float epsilonMaxi) {
  // --------- HANDOUT  PS04 ------------------------------
  // Generate a weight image that indicates which pixels are good to use in
  // HDR, i.e. weight=1 when the pixel value is in [epsilonMini, epsilonMaxi].
  // The weight is per pixel, per channel.
  Image output(im.width(), im.height(), im.channels()); // Initialize output image
  for (int h = 0; h < output.height(); h++) {
    for (int w = 0; w < output.width(); w++) {
      for (int c = 0; c < output.channels(); c++) {
        if ((im(w, h, c) <= 0.99) && (im(w, h, c) >= 0.002)) {
          output(w, h, c) = 1.0f; // 1 if inside range
        }
        else {
          output(w, h, c) = 0.0f; // 0 otherwise
        }
      }
    }
  }
  return output;
}

float computeFactor(const Image &im1, const Image &w1, const Image &im2,
                    const Image &w2) {
  // --------- HANDOUT  PS04 ------------------------------
  // Compute the multiplication factor between a pair of images. This
  // gives us the relative exposure between im1 and im2. It is computed as
  // the median of im2/(im1+eps) for some small eps, taking into account
  // pixels that are valid in both images.
  vector<float> f_vec;
  Image im1_adj = im1 + 0.0000000001; // Add correction value for divide by zero errors
  for (int h = 0; h < im1.height(); h++) {
    for (int w = 0; w < im1.width(); w++) {
      for (int c = 0; c < im1.channels(); c++) {
        if ((w1(w, h, c) > 0.9) && (w2(w, h, c) > 0.9)) { // If weights for both are 1 at this pixel
          f_vec.push_back(im2(w, h, c) / im1_adj(w, h, c)); // Add quotient to factor
        }
      }
    }
  }
  sort(f_vec.begin(), f_vec.end());
  cout << "Vector size" << f_vec.size() << endl;
  cout << "Max of vector: " << f_vec[f_vec.size() - 1] << endl;
  cout << "Min of vector: " << f_vec[0] << endl;
  if (f_vec.size() % 2 == 0) { // Take median of even number of elements vector
  
    return (f_vec[floor(f_vec.size() / 2)] + f_vec[floor(f_vec.size() / 2 - 1)]) / 2;
  }
  else { // Take median of odd number of elements vector
    return f_vec[floor(f_vec.size() / 2)];
  }
  // return factor;
}

Image makeHDR(vector<Image> &imSeq, float epsilonMini, float epsilonMaxi) {
  // --------- HANDOUT  PS04 ------------------------------
  // Merge images to make a single hdr image
  // For each image in the sequence, compute the weight map (special cases
  // for the first and last images).
  // Compute the exposure factor for each consecutive pair of image.
  // Write the valid pixel to your hdr output, taking care of rescaling them
  // properly using the factor.
  return Image(1, 1, 1);
}

/**************************************************************
 //                      TONE MAPPING                        //
 *************************************************************/

Image toneMap(const Image &im, float targetBase, float detailAmp, bool useBila,
              float sigmaRange) {
  // --------- HANDOUT  PS04 ------------------------------
  // tone map an hdr image
  // - Split the image into its luminance-chrominance components.
  // - Work in the log10 domain for the luminance
  // -
  return im;
}

/*********************************************************************
 *                       Tone mapping helpers                        *
 *********************************************************************/

// image --> log10Image
Image log10Image(const Image &im) {
  // --------- HANDOUT  PS04 ------------------------------
  // Taking a linear image im, transform to log10 scale.
  // To avoid infinity issues, make any 0-valued pixel be equal the the
  // minimum non-zero value. See image_minnonzero(im).
  return im;
}

// Image --> 10^Image
Image exp10Image(const Image &im) {
  // --------- HANDOUT  PS04 ------------------------------
  // take an image in log10 domain and transform it back to linear domain.
  // see pow(a, b)
  return im;
}

// min non-zero pixel value of image
float image_minnonzero(const Image &im) {
  // --------- HANDOUT  PS04 ------------------------------
  // return the smallest value in the image that is non-zeros (across all
  // channels too)
  return 0.0f;
}
