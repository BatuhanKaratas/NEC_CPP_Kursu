#include <iostream>
#include "date.h"

int main() {
    try
    {
        srand(time(nullptr));
        project::Date a{1, 1, 1899};
    }
    catch (const std::exception & ex)
    {
        std::cout << ex.what() << '\n';
    }
    catch (...)
    {
        std::cout << "Unknown error"  << '\n';
    }

    return 0;
}
