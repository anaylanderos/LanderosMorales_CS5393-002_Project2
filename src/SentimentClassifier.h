#ifndef SENTIMENTCLASSIFIER_H
#define SENTIMENTCLASSIFIER_H

#include "DSString.h"
#include <vector>
#include <map>

class SentimentClassifier {
private:
    // Data structures to store word frequencies
    std::map<DSString, int> positiveWords;
    std::map<DSString, int> negativeWords;
    
    // Helper methods
    std::vector<DSString> tokenizeText(const DSString& text);
    void processTrainingTweet(const DSString& text, int sentiment);
    int predictSentiment(const DSString& text);

public:
    SentimentClassifier();
    
    // Main functions as outlined in the assignment
    void train(const DSString& trainingFile);
    void predict(const DSString& testingFile, const DSString& outputFile);
    void evaluatePredictions(const DSString& groundTruthFile, const DSString& outputFile);
};

#endif