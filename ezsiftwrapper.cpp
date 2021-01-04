#include "emscripten.h"

#include "ezsift.h"

#include <stdio.h>
#include <list>


extern "C" {

typedef unsigned char uchar;
typedef unsigned long ulong;

ulong EMSCRIPTEN_KEEPALIVE createImage(int w, int h) {
   ezsift::Image<uchar> *image = new ezsift::Image<uchar>(w, h);
   return (ulong)image;
}

void EMSCRIPTEN_KEEPALIVE disposeImage(ulong img) {
   ezsift::Image<uchar> *image = (ezsift::Image<uchar> *)img;
   delete image;
}

void EMSCRIPTEN_KEEPALIVE putPixel(ulong img, int x, int y, uchar val) {
   ezsift::Image<uchar> *image = (ezsift::Image<uchar> *)img;
   image->data[y * image->w + x] = val;
}

uchar EMSCRIPTEN_KEEPALIVE getPixel(ulong img, int x, int y) {
   ezsift::Image<uchar> *image = (ezsift::Image<uchar> *)img;
   return image->data[y * image->w + x];
}


#define MAX_CACHE_N 10
std::list<ezsift::SiftKeypoint> rKeypointsCache[MAX_CACHE_N];
std::list<ezsift::SiftKeypoint> rKeypoints;
std::list<ezsift::SiftKeypoint>::iterator rKpiter;

void EMSCRIPTEN_KEEPALIVE extractFeature(ulong img) {
    ezsift::Image<uchar> *image = (ezsift::Image<uchar> *)img;
    rKeypoints.clear();
    bool bExtractDescriptor = true;

    // Double the original image as the first octive.
    ezsift::double_original_image(true);

    // Perform SIFT computation on CPU.
    printf("Start SIFT detection ...\n");
    ezsift::sift_cpu(*image, rKeypoints, bExtractDescriptor);
    printf("Total keypoints number: %u\n", static_cast<unsigned int>(rKeypoints.size()));
}

void EMSCRIPTEN_KEEPALIVE backupKeypoints(int i) {
   if (i < 0 || i >= MAX_CACHE_N) return;
   rKeypointsCache[i].clear();
   rKeypointsCache[i] = rKeypoints;
}

void EMSCRIPTEN_KEEPALIVE restoreKeypoints(int i) {
   if (i < 0 || i >= MAX_CACHE_N) return;
   rKeypoints.clear();
   rKeypoints = rKeypointsCache[i];
}

int EMSCRIPTEN_KEEPALIVE getKeypointsN() {
   return static_cast<int>(rKeypoints.size());
}

void EMSCRIPTEN_KEEPALIVE keypointBegin() {
   rKpiter = rKeypoints.begin();
}

void EMSCRIPTEN_KEEPALIVE keypointNext() {
   rKpiter ++;
}

int EMSCRIPTEN_KEEPALIVE keypointHasNext() {
   return rKeypoints.end() != rKpiter?1:0;
}

int EMSCRIPTEN_KEEPALIVE keypointOctave() {
   return rKpiter->octave;
}

int EMSCRIPTEN_KEEPALIVE keypointLayer() {
   return rKpiter->layer;
}

// c (x), r (y), scale (r), ori (theta)

double EMSCRIPTEN_KEEPALIVE keypointR() {
   return rKpiter->r;
}

double EMSCRIPTEN_KEEPALIVE keypointC() {
   return rKpiter->c;
}

double EMSCRIPTEN_KEEPALIVE keypointScale() {
   return rKpiter->scale;
}

double EMSCRIPTEN_KEEPALIVE keypointOri() {
   return rKpiter->ori;
}

int EMSCRIPTEN_KEEPALIVE keypointDescriptor(int i) {
   if (i < 0 || i >= 128) return 0;
   return rKpiter->descriptors[i];
}

std::list<ezsift::MatchPair> rMatch;
std::list<ezsift::MatchPair>::iterator rMtiter;
void EMSCRIPTEN_KEEPALIVE keypointsMatch(int a, int b) {
    if (a < 0 || a >= MAX_CACHE_N || b < 0 || b >= MAX_CACHE_N) return;
    std::list<ezsift::SiftKeypoint> kpt_list1 = rKeypointsCache[a];
    std::list<ezsift::SiftKeypoint> kpt_list2 = rKeypointsCache[b];

    // Match keypoints.
    rMatch.clear();
    ezsift::match_keypoints(kpt_list1, kpt_list2, rMatch);

    printf("Total matched keypoints number: %u\n", static_cast<unsigned int>(rMatch.size()));
}

int EMSCRIPTEN_KEEPALIVE getMatchN() {
   return static_cast<int>(rMatch.size());
}

void EMSCRIPTEN_KEEPALIVE matchBegin() {
   rMtiter = rMatch.begin();
}

void EMSCRIPTEN_KEEPALIVE matchNext() {
   rMtiter ++;
}

int EMSCRIPTEN_KEEPALIVE matchHasNext() {
   return rMatch.end() != rMtiter?1:0;
}

int EMSCRIPTEN_KEEPALIVE matchR1() {
   return rMtiter->r1;
}

int EMSCRIPTEN_KEEPALIVE matchR2() {
   return rMtiter->r2;
}

int EMSCRIPTEN_KEEPALIVE matchC1() {
   return rMtiter->c1;
}

int EMSCRIPTEN_KEEPALIVE matchC2() {
   return rMtiter->c2;
}


} // extern "C"

int main() { return 0; }
