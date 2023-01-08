#include "OrderBook.h"

OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}

std::vector<std::string> OrderBook::getKnownProducts(std::string time)
{
    std::vector<std::string> products;

    std::map<std::string,bool> prodMap;

    for (OrderBookEntry& e : orders[time])
    {
        prodMap[e.product] = true;
    }
    
    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}

std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                        std::string product,
                                        std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
        
    // loop through orders map
    for (OrderBookEntry& e : orders[timestamp])
    {
        //find orders
        if (e.orderType == type &&
            e.product == product)
            {
                //store orders
                orders_sub.push_back(e);
            }
    }
    return orders_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    
    for (OrderBookEntry& e : orders)
    {
        if (e.price > max)max = e.price;
    }
    return max;
}


double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
    double min = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price < min)min = e.price;
    }
    return min;
}

double OrderBook::getAvgPrice(std::vector<OrderBookEntry>& orders)
{
    int numberOfOrders = 0;
    
    double sum = 0;
    for (OrderBookEntry& e : orders)
    {
        sum += e.price;
        numberOfOrders ++;
    }
    return sum/numberOfOrders ;
}


std::string OrderBook::getEarliestTime()
{
    
    // Get first element in map orders
    return orders.begin()->first;
    
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    
    // loop through orders map
    
    for (auto const& t : orders)
    {
        // find next timestamp
        
        if(t.first > timestamp)
        {
            next_timestamp = t.first;
            break;
        }
    }
    
    // end of orders map
    
    if (next_timestamp == "")
    {
        next_timestamp = orders.begin()->first;
    }
    return next_timestamp;
}

std::string OrderBook::getLastTime(std::string timestamp)
{
    std::string next_timestamp = "";
    
    // loop through orders map
    
    for (auto const& t : orders)
    {
        // find last timestamp
        
        if(t.first < timestamp)
        {
            next_timestamp = t.first;
            break;
        }
    }
    
    // start of orders map
    
    if (next_timestamp == "")
    {
        next_timestamp = orders.rbegin()->first;
    }
    return next_timestamp;
    
}
