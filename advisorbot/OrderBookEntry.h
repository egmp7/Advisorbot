#pragma once

#include <string>

enum class OrderBookType{bid, ask, unknown};

class OrderBookEntry
{
    public:
    
        // Constructor
    
        OrderBookEntry( double _price, 
                        double _amount, 
                        std::string _timestamp, 
                        std::string _product, 
                        OrderBookType _orderType);
    
        double price;
        double amount;
        std::string timestamp;
        std::string product;
        OrderBookType orderType;
    
        // functions

        static OrderBookType stringToOrderBookType(std::string s);

        static bool compareByPriceAsc(OrderBookEntry& e1, OrderBookEntry& e2)
        {
            return e1.price < e2.price;
        }
         static bool compareByPriceDesc(OrderBookEntry& e1, OrderBookEntry& e2)
        {
            return e1.price > e2.price;
        }

        
};
