#ifndef Advisorbot_hpp
#define Advisorbot_hpp

#include <stdio.h>
#include <iostream>
#include <map>

#include "CSVReader.h"
#include "OrderBook.h"
#include "OrderBookEntry.h"

class Advisorbot
{
    
    public:
        Advisorbot();
        void init();

    private:
    
        /** Get user input
         *@return input User Input
         */
        std::string getUserCommand();
        
        /** Process user input from command line
         *@param cmd User Input
         */
        void processUserCommand(std::string cmd);
    
        /** Prints a string with a list of commands*/
        void printCommandList();
    
        /** Prints a string with a additional informationt of a command*/
        void printCommandHelp();
    
        /** Prints a string with al the products in the current market*/
        void printAllProducts();
    
        /** Prints a string with the current time*/
        void printCurrentTime();
    
        /** Prints the minimum bid or ask for a specific  product in the current market */
        void printCurrentMin();
    
        /** Prints the maximum bid or ask for a specific  product in the current market */
        void printCurrentMax();
    
        /** Prints the average bid or ask for a product in the last specified timesteps of the market */
        void printPastAvg();
    
        /** Prints the average bid or ask for a product in the following timesteps of the market */
        void printPrediction();
    
        /** Prints the median bid or ask for a product in the current market */
        void printCurrentMedian();
    
        /** Finds the next time frame in the OrderBook */
        void gotoNextTimeframe(); 
    
        
        OrderBook orderBook{"20200601.csv"};
        CSVReader csvReader;
        
        std::string currentTime;
        std::map<std::string,std::string> commands;
        std::vector<std::string> cmd;
};

#endif /* Advisorbot_hpp */
