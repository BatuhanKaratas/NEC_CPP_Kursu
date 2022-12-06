#include <iostream>
#include "date.h"

int main() {
    try
    {
        srand(time(nullptr));

        for (int i = 0; i < 5; ++i) {
            std::cout << project::Date::random_date();
        }
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
