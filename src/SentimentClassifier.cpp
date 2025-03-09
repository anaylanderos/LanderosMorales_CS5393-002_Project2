#include "SentimentClassifier.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>

SentimentClassifier::SentimentClassifier() {
    // Constructor implementation
}

// Helper method to tokenize text into words
std::vector<DSString> SentimentClassifier::tokenizeText(const DSString& text) {
    std::vector<DSString> tokens;
    
    // Convert DSString to C-string for tokenization
    const char* cstr = text.c_str();
    
    // Use a std::stringstream to tokenize
    std::stringstream ss(cstr);
    std::string token;
    
    // Extract words using spaces and punctuation as delimiters
    while (ss >> token) {
        // Remove punctuation and convert to lowercase
        std::string cleanToken;
        for (char c : token) {
            if (isalpha(c)) {
                cleanToken += tolower(c);
            }
        }
        
        // Only add non-empty tokens
        if (!cleanToken.empty()) {
            tokens.push_back(DSString(cleanToken.c_str()));
        }
    }
    
    return tokens;
}

// Process a single training tweet
void SentimentClassifier::processTrainingTweet(const DSString& text, int sentiment) {
    // Tokenize the tweet
    std::vector<DSString> words = tokenizeText(text);
    
    // Update word frequencies based on sentiment
    for (const DSString& word : words) {
        if (sentiment == 4) { // Positive sentiment
            positiveWords[word]++;
        } else { // Negative sentiment (0)
            negativeWords[word]++;
        }
    }
}

// Predict sentiment for a single tweet
int SentimentClassifier::predictSentiment(const DSString& text) {
    // Tokenize the tweet
    std::vector<DSString> words = tokenizeText(text);
    
    // Count positive and negative word occurrences
    int positiveScore = 0;
    int negativeScore = 0;
    
    for (const DSString& word : words) {
        // Check for word in positive dictionary
        auto posIt = positiveWords.find(word);
        if (posIt != positiveWords.end()) {
            positiveScore += posIt->second;
        }
        
        // Check for word in negative dictionary
        auto negIt = negativeWords.find(word);
        if (negIt != negativeWords.end()) {
            negativeScore += negIt->second;
        }
    }
    
    // Compare scores to determine sentiment
    if (positiveScore > negativeScore) {
        return 4; // Positive sentiment
    } else {
        return 0; // Negative sentiment
    }
}

// Train the classifier on the training dataset
void SentimentClassifier::train(const DSString& trainingFile) {
    std::ifstream file(trainingFile.c_str());
    
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open training file " << trainingFile.c_str() << std::endl;
        return;
    }
    
    std::string line;
    
    // Process each line in the training file
    while (std::getline(file, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        // Parse CSV format: sentiment,id,date,query,username,text
        std::stringstream ss(line);
        std::string token;
        
        // Extract sentiment (first field)
        std::getline(ss, token, ',');
        int sentiment;
        try {
            sentiment = std::stoi(token);
        } catch (...) {
            // Skip header line or lines with invalid sentiment
            continue;
        }
        
        // Only process tweets with sentiment 0 or 4
        if (sentiment != 0 && sentiment != 4) {
            continue;
        }
        
        // Skip id, date, query, username fields
        for (int i = 0; i < 4; i++) {
            std::getline(ss, token, ',');
        }
        
        // Get the tweet text (rest of the line)
        std::string tweetText;
        std::getline(ss, tweetText);
        
        // Extract the actual text which may be contained in quotes
        if (tweetText.size() >= 2 && tweetText.front() == '"' && tweetText.back() == '"') {
            tweetText = tweetText.substr(1, tweetText.size() - 2);
        }
        
        // Process the tweet
        processTrainingTweet(DSString(tweetText.c_str()), sentiment);
    }
    
    file.close();
}

// Predict sentiment for test dataset and write results to output file
void SentimentClassifier::predict(const DSString& testingFile, const DSString& outputFile) {
    std::ifstream testFile(testingFile.c_str());
    std::ofstream resultFile(outputFile.c_str());
    
    if (!testFile.is_open()) {
        std::cerr << "Error: Unable to open testing file " << testingFile.c_str() << std::endl;
        return;
    }
    
    if (!resultFile.is_open()) {
        std::cerr << "Error: Unable to open output file " << outputFile.c_str() << std::endl;
        testFile.close();
        return;
    }
    
    std::string line;
    
    // Process each line in the testing file
    while (std::getline(testFile, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        // Parse CSV format: id,date,query,username,text
        std::stringstream ss(line);
        std::string token;
        
        // Extract tweet ID (first field)
        std::getline(ss, token, ',');
        std::string tweetId = token;
        
        // Skip date, query, username fields
        for (int i = 0; i < 3; i++) {
            std::getline(ss, token, ',');
        }
        
        // Get the tweet text (rest of the line)
        std::string tweetText;
        std::getline(ss, tweetText);
        
        // Extract the actual text which may be contained in quotes
        if (tweetText.size() >= 2 && tweetText.front() == '"' && tweetText.back() == '"') {
            tweetText = tweetText.substr(1, tweetText.size() - 2);
        }
        
        // Predict sentiment for this tweet
        int sentiment = predictSentiment(DSString(tweetText.c_str()));
        
        // Write prediction to output file
        resultFile << sentiment << ", " << tweetId << std::endl;
    }
    
    testFile.close();
    resultFile.close();
}

// Evaluate predictions against ground truth and calculate accuracy
void SentimentClassifier::evaluatePredictions(const DSString& groundTruthFile, const DSString& outputFile) {
    // Maps to store ground truth and predictions
    std::map<std::string, int> groundTruth;
    std::map<std::string, int> predictions;
    
    // Read ground truth file
    std::ifstream truthFile(groundTruthFile.c_str());
    if (!truthFile.is_open()) {
        std::cerr << "Error: Unable to open ground truth file " << groundTruthFile.c_str() << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(truthFile, line)) {
        // Parse CSV format: sentiment, id
        std::stringstream ss(line);
        std::string sentiment, id;
        
        std::getline(ss, sentiment, ',');
        std::getline(ss, id);
        
        // Remove whitespace
        id.erase(0, id.find_first_not_of(" \t"));
        
        try {
            groundTruth[id] = std::stoi(sentiment);
        } catch (...) {
            // Skip invalid lines
            continue;
        }
    }
    truthFile.close();
    
    // Read predictions file
    std::ifstream predFile("results.csv");
    if (!predFile.is_open()) {
        std::cerr << "Error: Unable to open results file results.csv" << std::endl;
        return;
    }
    
    while (std::getline(predFile, line)) {
        // Parse CSV format: sentiment, id
        std::stringstream ss(line);
        std::string sentiment, id;
        
        std::getline(ss, sentiment, ',');
        std::getline(ss, id);
        
        // Remove whitespace
        id.erase(0, id.find_first_not_of(" \t"));
        
        try {
            predictions[id] = std::stoi(sentiment);
        } catch (...) {
            // Skip invalid lines
            continue;
        }
    }
    predFile.close();
    
    // Calculate accuracy and write to output file
    std::ofstream accFile(outputFile.c_str());
    if (!accFile.is_open()) {
        std::cerr << "Error: Unable to open accuracy file " << outputFile.c_str() << std::endl;
        return;
    }
    
    // Count correct predictions
    int correct = 0;
    int total = 0;
    std::vector<std::string> incorrectIds;
    
    for (const auto& pair : groundTruth) {
        const std::string& id = pair.first;
        int truth = pair.second;
        
        // If we have a prediction for this tweet
        if (predictions.find(id) != predictions.end()) {
            int prediction = predictions[id];
            total++;
            
            if (prediction == truth) {
                correct++;
            } else {
                // Store incorrect predictions
                incorrectIds.push_back(id);
            }
        }
    }
    
    // Calculate accuracy
    double accuracy = 0.0;
    if (total > 0) {
        accuracy = static_cast<double>(correct) / total;
    }
    
    // Write accuracy to file (3 decimal places)
    accFile << std::fixed << std::setprecision(3) << accuracy << std::endl;
    
    // Write incorrect predictions
    for (const std::string& id : incorrectIds) {
        accFile << predictions[id] << ", " << groundTruth[id] << ", " << id << std::endl;
    }
    
    accFile.close();
}