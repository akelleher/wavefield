#include <iostream>
#include <math.h>
#include <vector>
#include <cmath>

std::vector<float> getWavefieldDelays(float sourceX,float sourceY, int speakerCount, float speakerSpacing);
std::vector<float> getWavefieldGains(float sourceX,float sourceY, int speakerCount, float speakerSpacing);
std::vector<float> getSpeakerDistanceFromSource(float sourceX,float sourceY, int speakerCount, float speakerSpacing);
void computeSincTable(std::vector<float> * table, int sampleRate, int chunkSize, int upscaleFactor);
float sinc(float x);

float pythagorean(float x1, float y1, float x2, float y2);