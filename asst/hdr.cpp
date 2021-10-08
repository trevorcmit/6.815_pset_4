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
        if ((im(w, h, c) <= epsilonMaxi) && (im(w, h, c) >= epsilonMini)) {
          output(w, h, c) = 1.0f; // 1 if inside range
        }
        else {
          output(w, h, c) = 0.0f; // 0 otherwise
        }
      }
    }
  }
  return output; // Return weight image
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
  if (f_vec.size() % 2 == 0) { // Take median of even number of elements vector
    return (f_vec[floor(f_vec.size() / 2)] + f_vec[floor(f_vec.size() / 2 - 1)]) / 2;
  }
  else { // Take median of odd number of elements vector
    return f_vec[floor(f_vec.size() / 2)];
  }
}

Image makeHDR(vector<Image> &imSeq, float epsilonMini, float epsilonMaxi) {
  // --------- HANDOUT  PS04 ------------------------------
  // Merge images to make a single hdr image
  // For each image in the sequence, compute the weight map (special cases
  // for the first and last images).
  // Compute the exposure factor for each consecutive pair of image.
  // Write the valid pixel to your hdr output, taking care of rescaling them
  // properly using the factor.
  Image output(imSeq.at(0).width(), imSeq.at(0).height(), imSeq.at(0).channels()); // Initialize output

  vector<float> k_i;            // Initialize vector to hold k_i values
  k_i.push_back(1.0f);          // Add 1.0 as first factor
  vector<Image> weights;        // Initialize vector to store weights so we don't compute multiple times
  Image weight_0 = computeWeight(imSeq.at(0), epsilonMini, 1.0f); // First image weight
  weights.push_back(weight_0);  // Push first weight onto vector

  for (int n = 1; n < imSeq.size(); n++) { // Iterate over all and make factor compared to first image
    if (n == imSeq.size() - 1) {
      Image weight = computeWeight(imSeq.at(n), 0.0f, epsilonMaxi); // In brightest case, don't chop darkest
      k_i.push_back(computeFactor(imSeq.at(n - 1), weights.at(weights.size() - 1), imSeq.at(n), weight));
      weights.push_back(weight);
    }
    else {
      Image weight = computeWeight(imSeq.at(n), epsilonMini, epsilonMaxi);
      k_i.push_back(computeFactor(imSeq.at(n - 1), weights.at(weights.size() - 1), imSeq.at(n), weight));
      weights.push_back(weight);
    }
  }

  for (int h = 0; h < output.height(); h++) { // Iterate to create output image
    for (int w = 0; w < output.width(); w++) {
      for (int c = 0; c < output.channels(); c++) {

        float denom = 0.0f, sum = 0.0f;
        for (int n = 0; n < imSeq.size(); n++) {
          denom += weights.at(n)(w, h, c);
          sum += weights.at(n)(w, h, c) * (1.0f / k_i.at(n)) * imSeq.at(n)(w, h, c);
        }
        output(w, h, c) = sum / denom;

      }
    }
  }

  return output; // Return output HDR image
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
