#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>

// liberalBlocks Function is to split a vector of strings into blocks based on empty strings
std::vector<std::vector<std::string>> liberalBlocks(const std::vector<std::string>& b)
{
    std::vector<std::vector<std::string>> liberalBlocks;
    std::vector<std::string> currentBlock;

    // Iterate through the loop over input vector of strings
     for (const auto& Getvalue : b) 
    { 

    // if the string is empty and current block is not empty it will move current block to the LiberalBlocks else it will return the string from current block
        if (Getvalue.empty()) 
        {
            if (!currentBlock.empty()) 
            {
                liberalBlocks.push_back(std::move(currentBlock));
            }
        } 
        else 
        {
            currentBlock.push_back(Getvalue);
        }
    }

    // Add the last block if not empty
    if (!currentBlock.empty())
    {
        liberalBlocks.push_back(std::move(currentBlock));
    }

    return liberalBlocks;
}

// pruneCentrist Function to prune values in liberal blocks based on a threshold similarity score
std::set<std::string> pruneCentrist(const std::vector<std::string>& t, const std::vector<std::string>& C)
{
    std::set<std::string> SimilarityValues;

    // Iterate through liberal blocks obtained from vector t
    for (const auto& LiB : liberalBlocks(t))
    {
        // Calculate the similarity score by counting values present in both LiB and vector C
        double score = static_cast<double>(std::count_if(LiB.begin(), LiB.end(), [&C](const std::string& val)
        {
            return std::find(C.begin(), C.end(), val) != C.end();
        })) / LiB.size();

        // If the similarity score is above 0.75, add the values to the set, which means it has signficant similairty the other column
        if (score > 0.75)
        {
            SimilarityValues.insert(LiB.begin(), LiB.end());
        }
    }

    return SimilarityValues;
}

int main()
{
    // Specify the file location for the data input
    std::string fileLocation = "C:\\Users\\Srinivas Tarun\\Downloads\\datasets.data-cityofnewyork-us.education\\tsv\\2pmj-y4p4.tsv\\2pmj-y4p4.tsv";

    // Open the file
    std::ifstream file(fileLocation);
    if (!file.is_open())
    {
        // Display an error message if file failes to open
        std::cerr << "Error while file opening" << std::endl;
        return 1;
    }

    std::string header;
    std::getline(file, header);

    std::vector<std::string> t, C;
    std::string line;
    // Read the file each line
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;
        int columnNum = 0;

        // Tokenize each line based on tab ('\t') delimiter
        while (std::getline(iss, token, '\t'))
        {
            // Extract values from specific columns (2nd and 8th columns) and store them in vectors t and C
            if (columnNum == 2)
            {
                t.push_back(token);
            }
            else if (columnNum == 8)
            {
                C.push_back(token);
            }

            columnNum++;
        }
    }

    // Call the pruneCentrist function to identify similar values in liberal blocks
    std::set<std::string> result = pruneCentrist(t, C);

    // Display the result
    if (result.empty())
    {
        std::cout << "No similar values found." << std::endl;
    }
    else
    {
        std::cout << "Similar values after pruning: ";
        for (const std::string& val : result)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    // Close the file
    file.close();

    return 0;
}