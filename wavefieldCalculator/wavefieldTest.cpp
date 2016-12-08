// wavefieldTest.cpp
#include "wavefieldCalculator.h"


int main(){
    float sourceX = 0;
    float sourceY = 2;
    float speakerCount = 5;
    float speakerSpacing = .060375;
    // std::vector<float> distances = getSpeakerDistanceFromSource(sourceX, sourceY, speakerCount, speakerSpacing);
    // std::vector<float> delays = getWavefieldDelays(sourceX, sourceY, speakerCount, speakerSpacing);
    // std::vector<float> gains = getWavefieldGains(sourceX, sourceY, speakerCount, speakerSpacing);

    // std::cout << "Distances:" << std::endl;
    // for(int i = 0; i < distances.size(); i++){
    //     std::cout << distances[i] << " ";
    // }   
    // std::cout <<  std::endl;
    // std::cout << "Delays:" << std::endl;
    // for(int i = 0; i < delays.size(); i++){
    //     std::cout << delays[i] << " ";
    // }
    // std::cout <<  std::endl;
    // std::cout << "Gains:" << std::endl;
    // for(int i = 0; i < gains.size(); i++){
    //     std::cout << gains[i] << " ";
    // }
    // std::cout << std::endl;

    std::vector<float> * table = new std::vector<float>;
    computeSincTable(table, 48000, 100, 10);
    if(table){
        for(int i = 0; i < table->size(); i++){
            std::cout << (*table)[i] << " ";
        }
        std::cout << std::endl;
    }
}