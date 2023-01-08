#include "CSVReader.h"

CSVReader::CSVReader()
{

}

std::map<std::string, std::vector<OrderBookEntry>> CSVReader::readCSV(std::string csvFilename)
{
    
    // variables
    std::map<std::string, std::vector<OrderBookEntry>> entries;
    std::string line;
    
    //get file
    std::ifstream csvFile(csvFilename);
    if (csvFile.is_open())
    {
        // read each line
        while(std::getline(csvFile, line))
        {
            
            try
            {
                // store each line into an OrderBookEntry object
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                
                //store objects in a map
                entries[obe.timestamp].push_back(obe);
                
            }
            // catch error
            catch(const std::exception& e)
            {
                std::cout << "CSVReader::readCSV bad data"  << std::endl;
            }
        }// end of while
    }
    
    else std::cout << "csvFile failed to open file"  << std::endl; // file not open

    std::cout << "CSVReader::readCSV read " << entries.size() << " entries"  << std::endl;
    
    return entries;
}

// Same tokenise algorythm shown in class
std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    
   std::vector<std::string> tokens;
   int start, end;
   std::string token;

    start = csvLine.find_first_not_of(separator, 0);
    do{
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break;
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
    start = end + 1;
    }while(end > 0);
    
    return tokens;
}

// Same algorythm shown in class
OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    double price, amount;

    if (tokens.size() != 5) // bad
    {
        std::cout << "Bad line " << std::endl;
        throw std::exception{};
    }
    // we have 5 tokens
    try {
         price = std::stod(tokens[3]);
         amount = std::stod(tokens[4]);
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3]<< std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4]<< std::endl;
        throw;
    }
    
    OrderBookEntry obe{price,
                        amount,
                        tokens[0],
                        tokens[1],
                        OrderBookEntry::stringToOrderBookType(tokens[2])};

    return obe;
}
     
