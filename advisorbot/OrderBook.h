#pragma once

#include "CSVReader.h"
#include "OrderBook.h"
#include "OrderBookEntry.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>

class OrderBook
{
    public:
    /** construct, reading a csv data file */
        OrderBook(std::string filename);
    
    /** return vector of Orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                              std::string product,
                                              std::string timestamp);
    
    /** return vector of all know products in the dataset*/
        std::vector<std::string> getKnownProducts(std::string time);
    
    /** returns the earliest time in the orderbook*/
        std::string getEarliestTime();
    
    /** returns the next time after the
    *     sent time in the orderbook
    *     If there is no next timestamp, wraps around to the start */
        std::string getNextTime(std::string timestamp);
    
    /** returns the last time after the
    *     sent time in the orderbook
    *     If there is no next timestamp, wraps around to the start*/
        std::string getLastTime(std::string timestamp);
    
    /** find the hightest price in the orders*/
        static double getHighPrice(std::vector<OrderBookEntry>& orders);
    /** find the lowest price in the orders*/
        static double getLowPrice(std::vector<OrderBookEntry>& orders);
    /** find the average price in the orders*/
        static double getAvgPrice(std::vector<OrderBookEntry>& orders);
    
    private:
    
        std::map<std::string,std::vector<OrderBookEntry>> orders;

};
