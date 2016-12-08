#include "wavefieldCalculator.h"

//Finds distance from a point to linear array of speakers
std::vector<float> getSpeakerDistanceFromSource(float sourceX,float sourceY, int speakerCount, float speakerSpacing){
	//	Virtual source position - origin is center of array
	//float speakerSpacing = .060325; // distance between speakers in m
	// std::cout << pythagorean(0,0,0,0) << std::endl;
	std::vector<float> speakerDistanceFromSource;

	for(int i = 0; i < speakerCount; i++){
		float thisSpeakerPos = speakerSpacing*(float)(i)-speakerSpacing*(float)(speakerCount-1)/2.0;
		speakerDistanceFromSource.push_back(pythagorean(sourceX, sourceY, thisSpeakerPos, 0));
		// std::cout << speakerDistanceFromSource[i] << std::endl;
	}
	// std::cout << std::endl;
	return speakerDistanceFromSource;

}

std::vector<float> getWavefieldDelays(float sourceX,float sourceY, int speakerCount, float speakerSpacing){
	std::vector<float> distances = getSpeakerDistanceFromSource(sourceX, sourceY, speakerCount, speakerSpacing);
	std::vector<float> timeDelays;
	for(int i = 0; i < distances.size(); i++){
		timeDelays.push_back(distances[i]/340.3);	//distance to time
	}
	float min = -1;
	for(int i = 0; i < timeDelays.size(); i++){ //Find minimum
		if(min == -1){
			min = timeDelays[i];
		} else if (min > timeDelays[i]){
			min = timeDelays[i];
		}
	}
	for (int i = 0; i < timeDelays.size(); i++){	//	normalize time delays
		timeDelays[i] -= min;
	}
	return timeDelays;
}


std::vector<float> getWavefieldGains(float sourceX,float sourceY, int speakerCount, float speakerSpacing){
	std::vector<float> distances = getSpeakerDistanceFromSource(sourceX, sourceY, speakerCount, speakerSpacing);
	std::vector<float> gains;
	for(int i = 0; i < distances.size(); i++){
		gains.push_back(1/pow(distances[i], 2));	//	1/d^2 - assume spherical emission (inverse square law)
	}
	float maxGain = 0;
	for(int i = 0; i < gains.size(); i++){ //	Find max gain
		if(gains[i] > maxGain){
			maxGain = gains[i];
		}
	}
	// for(int i = 0; i < gains.size(); i++){
	// 	gains[i] /= maxGain;	//normalize so greatest gain is 1
	// }
	return gains;
}

float pythagorean(float x1, float y1, float x2, float y2){
	return sqrt(pow(x1-x2, 2)+pow(y1-y2, 2));
}

void computeSincTable(std::vector<float> * table, int sampleRate, int tableSize, int upscaleFactor) {
	table->clear();
	for (int i = -tableSize/2; i < tableSize/2; i++) {
		
		table->push_back(sinc((float)i/upscaleFactor));
		std::cout << sinc((float)i/upscaleFactor) << " \n";
	}
}

float sinc(float x) {
	if (x == 0.0) {
		return 1.0;
	}
	double pi = 3.14159265359;
	double y = pi*x;
	return (sin(y) / y);
}