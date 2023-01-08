#include "Advisorbot.hpp"

Advisorbot::Advisorbot()
{
    //Attributes of the commands map store help information.
    
    commands["help"] = "help -> list all commands";
    commands["prod"] = "prod -> list all products in the current market";
    commands["min"] = "min ETH/BTC ask -> find minimum bid or ask for product in current time step";
    commands["max"] = "max ETH/BTC ask -> find maximum bid or ask for product in current time step";
    commands["avg"] = "avg ETH/BTC bid 10 -> average ETH/BTC bid over last 10 time steps";
    commands["predict"] = "predict max ETH/BTC bid -> predict max or min / ask or bid for the sent product in the next ten timesteps";
    commands["time"] = "time -> state current time in dataset, i.e. which timeframe are we looking at";
    commands["step"] = "step -> move to next time step";
    commands["median"] = "median ETH/BTC bid -> find the median bid or ask for product in current time step";
}

void Advisorbot::init()
{
    currentTime = orderBook.getEarliestTime();

    while(true)
    {
        processUserCommand(getUserCommand());
    }
}

std::string Advisorbot::getUserCommand()
{
    std::string userCommand ;
    std::cout << "advisorbot> Please enter command: "<< std::endl;
    std::getline(std::cin, userCommand);
    
    std::cout << "user> " << userCommand << std::endl;
    
    return userCommand;
}

void Advisorbot::printCommandList()
{
    std::string line;
    
    // get all commands map keys
    for (auto const& c : commands)
    {
        
        line +=  c.first + ", " ;
        
        if (c.first == "help")
        {
            line +=   "help <cmd>, " ;
        }
    }
    
    // delete last two characters from string
    line.pop_back();
    line.pop_back();
    
    // print commands
    std::cout << "<advisorbot> The available commands are: ("<< line << ")"<< std::endl;
}

void Advisorbot::printCommandHelp()
{
    for (auto const& c :commands)
    {
        // chech if command exists on the list
        if (cmd[1] == c.first)
        {
            // print
            std::cout <<"advisorbot> " << c.second << std::endl;
            return;
        }

    }
    // error
    std::cout << "advisorbot> Error: Advisorbot::printCommandHelp() -> command not in the list, try: 'help <cmd>'" << std::endl;
}

void Advisorbot::printCurrentTime()
{
    std::cout << "advisorbot> Current Time: " << currentTime << std::endl;
}

void Advisorbot::printAllProducts()
{
    std::string products;
    
    // get all products
    for (std::string const& p : orderBook.getKnownProducts(currentTime))
    {
        products += p + ",";
    }
    
    // delete the two last characters of string
    products.pop_back();
    products.pop_back();
    
    // print
    std::cout << "advisorbot>" << products <<  std::endl;
}

void Advisorbot::printCurrentMin()
{
    std::string product = cmd[1];
    std::string type = cmd[2];
    
    std::vector<OrderBookEntry> entries = orderBook.getOrders(
                OrderBookEntry::stringToOrderBookType(type),
                product,
                currentTime);
    
    if (!entries.empty()) {
        // entries exist, print min
        std::cout << "advisorbot> " << product << " Min "<< type <<": "
                  << OrderBook::getLowPrice(entries) <<
        std::endl;
    }
    else
    {
        // error
        std::cout << "advisorbot> Error: Advisorbot::printMin() -> bad product or type input, check: 'help min'" <<std::endl;

    }

}

void Advisorbot::printCurrentMax()
{
    std::string product = cmd[1];
    std::string type = cmd[2];


    std::vector<OrderBookEntry> entries = orderBook.getOrders(
                                OrderBookEntry::stringToOrderBookType(type),
                                product,
                                currentTime);
    if (!entries.empty())
    {
        // entries exist, print max
        std::cout << "advisorbot> " << product << " Max "<< type <<": "
                  << OrderBook::getHighPrice(entries)  <<
        std::endl;
    }
    else
    {
        // error
        std::cout << "advisorbot> Error: Advisorbot::printMax() -> bad product or type input, check: 'help man'" <<std::endl;
    }
    
    
}
void Advisorbot::printPastAvg()
{
    std::string product = cmd[1];
    std::string type = cmd[2];
    std::string timestamp = currentTime;
    int timesteps;
    double sum = 0;
    
    try{
        // store integer
        timesteps = stoi(cmd[3]);
        if(timesteps <= 0)
        {
            // bad integer
            std::cout << "advisorbot> Error: Advisorbot::printFutureAvg() -> timesteps needs to be > 0, check: 'help avg'" <<std::endl;
            return;
        }
    }
    
    catch (const std::exception& e)
    {
        // not an integer
        std::cout << "advisorbot> Error: Advisorbot::printFutureAvg() -> timesteps is not an integer, check: 'help avg'" <<std::endl;
        return;
    }
    

    // get current entrie
    
    std::vector<OrderBookEntry> entries = orderBook.getOrders(
                                OrderBookEntry::stringToOrderBookType(type),
                                product,
                                timestamp);
    
        
        for (int i = 0; i < timesteps; i++)
        {
            
            // get entries
            
            entries = orderBook.getOrders(
                    OrderBookEntry::stringToOrderBookType(type),
                    product,
                    timestamp);
            
            
            //validation approved
            if (!entries.empty())
            {
                
                // get sum of avg
                sum += orderBook.getAvgPrice(entries);
                
                // last timeframe
                timestamp = orderBook.getLastTime(timestamp);
                
                
            }
            else
            {
                // error
                std::cout << "advisorbot> Error: Advisorbot::printFutureAvg() -> bad product or type input, check: 'help avg'" << std::endl;
                return;
            }
            
        }
    
        std::cout << "advisorbot> " << product << " average "<< type <<": "
                  << sum/timesteps << " in last " << timesteps <<" timesteps."<<
        std::endl;

}

void Advisorbot::printPrediction()
{
    std::string operation = cmd[1];
    std::string product = cmd[2];
    std::string type =cmd[3];
    std::string timestamp = currentTime;
    int timesteps = 10;
    float sum = 0;
    
    // validation of operation
    
    if (operation == "max" or operation == "min")
    {
        
        for (int i = 0; i < timesteps; i++)
        {
            
            std::vector<OrderBookEntry> entries = orderBook.getOrders(
                    OrderBookEntry::stringToOrderBookType(type),
                    product,
                    timestamp);
            
            //validation if entries exist;
            
            if (!entries.empty() )
            {
                // if max
                
                if(operation == "max")
                {
                    sum += OrderBook::getHighPrice(entries);

                }
                
                // if min
                
                else
                {
                    sum += OrderBook::getLowPrice(entries);
                }

                // next timeframe
                timestamp = orderBook.getNextTime(timestamp);
            }
            
            else
            {
                // error
                std::cout << "advisorbot> Error: Advisorbot::printPrediction() -> bad product or type input, check: 'help predict'" << std::endl;
                return;
            }
            
        }
        
        std::cout << "advisorbot> " << product << " Predicts: "<< type <<": "
                  << sum/timesteps << " in " << timesteps <<" timesteps."<<
        std::endl;
    }
    
    else
    {
        // error
        std::cout << "advisorbot> Error: Advisorbot::printPrediction() -> bad operation, use max or min. Check: 'help predict'" << std::endl;
    }
    
}

void Advisorbot::printCurrentMedian()
{
    std::string product = cmd[1];
    std::string type = cmd[2];
    
    std::vector<OrderBookEntry> entries = orderBook.getOrders(
                                OrderBookEntry::stringToOrderBookType(type),
                                product,
                                currentTime);
    
    if (!entries.empty())
    {
        // sort entries
        sort(entries.begin(),entries.end(),OrderBookEntry::compareByPriceAsc);
       
        // find median
        unsigned long median = entries.size()/2;
        
        //print
        std::cout << "advisorbot> " << product << " Median "<< type <<": "
                  << entries[median].price <<
        std::endl;
        
    }
    else
    {
        // error
        std::cout << "advisorbot> Error: Advisorbot::printCurrentMedian() -> bad product or type input, check: 'help median'" << std::endl;
    }
}

void Advisorbot::gotoNextTimeframe()
{
    std::cout << "advisorbot> past timeframe: " + currentTime << std::endl;
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "advisorbot> current timeframe: " + currentTime << std::endl;
}

void Advisorbot::processUserCommand(std::string input)
{
    // tokenise command line and store in cmd vector
    cmd = csvReader.tokenise(input,' ');
    
    
    if (cmd[0] == "help" && cmd.size() == 1)
    {
        printCommandList();
    }
    else if (cmd[0] == "help" && cmd.size() == 2)
    {
        printCommandHelp();
    }
    else if (cmd[0] == "prod" && cmd.size() == 1)
    {
        printAllProducts();
    }
    else if (cmd[0] == "min" && cmd.size() == 3)
    {
        printCurrentMin();
    }
    else if (cmd[0] == "max" && cmd.size() == 3)
    {
        printCurrentMax();
    }
    else if (cmd[0] == "avg" && cmd.size() == 4)
    {
        printPastAvg();
    }
    else if (cmd[0] == "predict" && cmd.size() == 4)
    {
        printPrediction();
    }
    else if (cmd[0] == "time" && cmd.size() == 1)
    {
        printCurrentTime();
    }
    else if (cmd[0] == "step" && cmd.size() == 1)
    {
        gotoNextTimeframe();
    }
    else if (cmd[0] == "median" && cmd.size() == 3)
    {
        printCurrentMedian();
    }
    else
    {
        std::cout << "advisorbot> Error:Advisorbot::processUserCommand() -> Command not found, try again." << std::endl;
    }
}
