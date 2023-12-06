//Hello
//This is our implementation of the column expansion algorithm

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm> // Included algorithm for find

// generate robust signatures function from a predefined dataset
std::unordered_map<std::string, std::unordered_set<std::string>> generateRobustSignatures(const std::vector<std::vector<std::string>>& documents) {
    std::unordered_map<std::string, std::unordered_set<std::string>> robustSignatures;

    for (const auto& document : documents) {
        if (document.size() < 2) {
            //checking for the size
            continue;
        }
//Taking in the term
        const std::string& term = document[0];
        //Iterating over the loop
        std::unordered_set<std::string> signature(document.begin() + 1, document.end());
//capturing the robust signatures
        robustSignatures[term] = signature;
    }

    return robustSignatures;
}

// Function to read data from the dataset file
std::vector<std::vector<std::string>> readTSV(const std::string& filename) {
    std::vector<std::vector<std::string>> D;

    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        //check if file is unable to open
        std::cerr << "Unable to unlock this file" << filename << std::endl;
        return D;
    }

    std::string line;
    if (std::getline(inputFile, line)) {
        //A simple if loop to check for the header
        std::istringstream headerStream(line);
        std::string term;
        std::vector<std::string> header;
        while (std::getline(headerStream, term, '\t')) {
            header.push_back(term);
        }
        D.push_back(header);
    }

    while (std::getline(inputFile, line)) {
        std::istringstream LS(line);
        std::vector<std::string> rec;
        std::string term;
        while (std::getline(LS, term, '\t')) {
            //performing records
            rec.push_back(term);
        }
        D.push_back(rec);
    }

    inputFile.close();
    return D;
}

// Function to perform column expansion using robust signatures
std::vector<std::string> expandColumn(
    const std::vector<std::string>& originalColumn,
    //mapping the robust signatures
    const std::unordered_map<std::string, std::unordered_set<std::string>>& robustSignatures,
    double thresholdSup,
    double thresholdCol) {
//Defining a expansion set
    std::unordered_set<std::string> expandedSet(originalColumn.begin(), originalColumn.end());

    std::string columnName = originalColumn[0];
    //Letting in the column name and then erasing it so that it won't record that.
    expandedSet.erase(columnName);

    while (thresholdCol > 0) {
        std::vector<std::string> expansionSet;

        for (const auto& termEntry : robustSignatures) {
            const std::string& term0 = termEntry.first;

            if (expandedSet.count(term0) == 0) {
                //checking if count>0 and comparing it with threshold value
                std::unordered_set<std::string> S;

                for (const std::string& term : originalColumn) {
                    if (termEntry.second.count(term) > thresholdSup) {
                        //inserting the term
                        S.insert(term0);
                    }
                }

                if (S.size() >= thresholdSup * termEntry.second.size() &&
                    S.size() > expandedSet.size() * thresholdCol) {
                        //Seeing if the values are right and computing the column_expansion for a term using the formula
                    expansionSet.push_back(term0);
                }
            }
        }

        if (!expansionSet.empty()) {
            expandedSet.insert(expansionSet.begin(), expansionSet.end());
            thresholdCol -= 0.05;
        } else {
            break;
        }
    }

    std::vector<std::string> expandedColumn(expandedSet.begin(), expandedSet.end());
    return expandedColumn;
}

// Function to calculate Precision
double calculatePrecision(const std::vector<std::string>& originalColumn, const std::vector<std::string>& expandedColumn) {
    int commonTerms = 0;
    for (const auto& term : expandedColumn) {
        if (std::find(originalColumn.begin(), originalColumn.end(), term) != originalColumn.end()) {
            commonTerms++;
        }
    }

    return static_cast<double>(commonTerms) / expandedColumn.size();
}

// Calculating the recall value for a column
double calculateRecall(const std::vector<std::string>& originalColumn, const std::vector<std::string>& expandedColumn) {
    int commonTerms = 0;
    // checking for common terms
    for (const auto& term : expandedColumn) {
        if (std::find(originalColumn.begin(), originalColumn.end(), term) != originalColumn.end()) {
            commonTerms++;
        }
    }

    return static_cast<double>(commonTerms) / originalColumn.size();
}

// Calculating the F1 score
double calculateF1Score(double precision, double recall) {
    //The formula to calculate the F1 score
    return (2 * precision * recall) / (precision + recall);
}

int main() {
    // Loading the dataset tsv file
    std::string filename = "C:\\Users\\Srinivas Tarun\\Downloads\\datasets.data-cityofnewyork-us.education\\tsv\\4n2j-ut8i.tsv\\4n2j-ut8i.tsv";

    std::vector<std::vector<std::string>> D = readTSV(filename);
// Loading the robust signatures text file
    std::string signaturesFilename = "C:\\Users\\Srinivas Tarun\\Desktop\\robust_signatures.txt";
    std::vector<std::vector<std::string>> signaturesData = readTSV(signaturesFilename);
    std::unordered_map<std::string, std::unordered_set<std::string>> robustSignatures = generateRobustSignatures(signaturesData);
//Initializing support threshold and column threshold same at the start
    double thresholdSup = 0.25;
    double thresholdCol = 0.25;

    for (int i = 0; i < D[0].size(); ++i) {
        std::vector<std::string> originalColumn;
        for (const auto& row : D) {
            originalColumn.push_back(row[i]);
        }
// Sending these values into the function
        std::vector<std::string> expandedColumn = expandColumn(originalColumn, robustSignatures, thresholdSup, thresholdCol);

        // Evaluating the 3 defined metrics
        double precision = calculatePrecision(originalColumn, expandedColumn);
        double recall = calculateRecall(originalColumn, expandedColumn);
        double f1Score = calculateF1Score(precision, recall);

        // Printing the original column
        std::cout << "Original Column (" << D[0][i] << "): ";
        for (const auto& term : originalColumn) {
            std::cout << term << " ";
        }
//Printing the main result the expanded column
        std::cout << "\nExpanded Column (" << D[0][i] << "): ";
        for (const auto& term : expandedColumn) {
            std::cout << term << " ";
        }
//Printing the evaluated metrics for each extended column
        std::cout << "\nPrecision: " << precision << " Recall: " << recall << " F1 Score: " << f1Score << "\n\n";
    }

    return 0;
}
//The end
//Thank you.