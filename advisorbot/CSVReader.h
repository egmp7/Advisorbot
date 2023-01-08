#pragma once

#include "OrderBookEntry.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

class CSVReader
{
    public:
        
        CSVReader();
    
    /** Read CSV file and return data as a map with each timestamp as key
     * @param csvFile String, CSV file name
     * @return orders Map with orders*/
        static std::map<std::string, std::vector<OrderBookEntry>> readCSV(std::string csvFile);
    
    /** Tokenise algorythm,
     * @param csvLine String, one line of the CSV file
     * @return tokens Vector of strings*/
        static std::vector<std::string> tokenise(std::string csvLine,
                                              char separator);
        
    private:
    
    /** Stores tokens into OrderBookEntry object,
     * @param strings Vector of string,
     * @return entries OrderBookEntry*/
        static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
         
};
