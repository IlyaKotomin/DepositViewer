#pragma region Includes And Usings

#include <iostream>

using namespace std;

#pragma endregion

#pragma region Exchange Rates

//16.11.2023 exchange rates
// ReSharper disable CppInconsistentNaming
constexpr double USD_TO_BLV = 1.80, USD_TO_EURO = 0.92;
// ReSharper restore CppInconsistentNaming

#pragma endregion

#pragma region Currency Types

enum currency
{
    // ReSharper disable CppInconsistentNaming
    USD,
    EURO,
    BGN,
    // ReSharper restore CppInconsistentNaming
};

#pragma endregion

#pragma region Exchange Methods

double get_rates_to_usd(const currency currency)
{
    if (currency == USD)
        return 1;
    else if (currency == EURO)
        return USD_TO_EURO;
    else
        return USD_TO_BLV;
}

double convert_money_by_rates(const double money, const currency from, const currency to)
{
    const double from_rate = get_rates_to_usd(from);
    const double  to_rate = get_rates_to_usd(to);

    return money * (to_rate / from_rate);
}

#pragma endregion

#pragma region Depositors

constexpr int max_depositors = 50;

struct depositor
{
    string name;
    
    // ReSharper disable CppInconsistentNaming
    double USD;
    double EURO;
    double BGN;
    // ReSharper restore CppInconsistentNaming
};

#pragma endregion

#pragma region Menu Pages

//1) Main Menu
int main_menu_handler()
{
    return 0; 
}

#pragma endregion

int main(int argc, char* argv[])
{
    while (true)
        if (main_menu_handler() == 0)
            break;
    
    return 0;
}
