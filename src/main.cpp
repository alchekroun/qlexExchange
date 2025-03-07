#include <iostream>

#include "exchange/Exchange.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;
	qlexexchange::Exchange exchange(60000);
	exchange.start();


	while (true) {
		exchange.update(-1, true);
	}
    return 0;
}