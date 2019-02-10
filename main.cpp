#include "fulkerson.hpp"

#include <iostream>

int main()
{
    falkerson_transport_net obj;

    obj.run_max_flow();

    std::cout << obj.get_max_flow() << std::endl;

    std::cout << "Optimal distribution is " << std::endl;

    obj.print_flows_lexicographically();

    return 0;
}