#include "DSString.h"
#include "SentimentClassifier.h"
#include <iostream>

int main(int argc, char** argv) {
    // Check for correct number of command line arguments
    if (argc != 6) {
        std::cout << "Usage: " << argv[0] << " <training_file> <testing_file> <testing_ground_truth> <results_file> <accuracy_file>" << std::endl;
        return 1;
    }
    
    // Get file names from command line arguments
    DSString trainingFile(argv[1]);
    DSString testingFile(argv[2]);
    DSString groundTruthFile(argv[3]);
    DSString resultsFile(argv[4]);
    DSString accuracyFile(argv[5]);
    
    // Create classifier
    SentimentClassifier classifier;
    
    // Train, predict, and evaluate
    std::cout << "Training classifier..." << std::endl;
    classifier.train(trainingFile);
    
    std::cout << "Predicting sentiment..." << std::endl;
    classifier.predict(testingFile, resultsFile);
    
    std::cout << "Evaluating predictions..." << std::endl;
    classifier.evaluatePredictions(groundTruthFile, accuracyFile);
    
    std::cout << "Done! Results written to " << resultsFile.c_str() << " and " << accuracyFile.c_str() << std::endl;
    
    return 0;
}