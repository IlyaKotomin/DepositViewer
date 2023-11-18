#pragma region Includes And Usings

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

#pragma endregion

#pragma region Program Handler Parameters

enum handler_codes
{
    exit_from_program = 0,
    continue_to_handle = 1,
};

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

bool compare_by_currency(const depositor& a, const depositor& b, const currency& currency)
{
    switch (currency)
    {
    case USD:
        return a.USD > b.USD;
    case EURO:
        return a.EURO > b.EURO;
    case BGN:
        return a.BGN > b.BGN;
    default: return false;  // NOLINT(clang-diagnostic-covered-switch-default)
    }
}

void sort_depositors_by_currency(depositor depositors[], currency currency)
{
    sort(depositors, depositors + max_depositors, [currency](const depositor& a, const depositor& b)
    {
        return compare_by_currency(a, b, currency);
    });
}

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

// ReSharper disable once CppInconsistentNaming
double convert_all_to_BGN(const depositor& depositor)
{
    double result = 0;

    result += convert_money_by_rates(depositor.USD, USD, BGN);
    result += convert_money_by_rates(depositor.EURO, EURO, BGN);
    return result;    
}

#pragma endregion

#pragma region File System

int get_file_lines_count(ifstream& file_stream)
{
    int lines = 0;
    string buffer;
    
    while (getline(file_stream, buffer))
        lines++;

    return lines;
}

void load_depositors_from_file(depositor depositors[], const string& path, int& depositors_counter)
{
    string line;
    ifstream file_stream(path);
    
    while (getline(file_stream, line))
    {
        istringstream string_stream(line);

        string formatted_name;
        
        string_stream >> formatted_name
                      >> depositors[depositors_counter].USD
                      >> depositors[depositors_counter].EURO
                      >> depositors[depositors_counter].BGN;
        
        replace(formatted_name.begin(), formatted_name.end(), '_', ' ');
        
        depositors[depositors_counter].name = formatted_name; 
        
        depositors_counter++;
    }
}

#pragma endregion

#pragma region Menu Pages

void wait_user_respond_and_clear()
{
    cout << endl;
    system("PAUSE");    // NOLINT(concurrency-mt-unsafe)
    system("CLS");      // NOLINT(concurrency-mt-unsafe)
}

int get_main_menu_choice()
{
    int choice;

    cout << "Menu:\n"
            "1. Create Depositor\n"
            "2. Show All Depositors\n"
            "3. Search Depositor By Name\n"
            "4. Show All Depositors Above Set Amount\n"
            "5. Sort By Currency\n"
            "6. EXIT AND SAVE\n\n"
            "Enter your choice: ";
    cin >> choice;

    return choice;
}

int get_sort_menu_handle()
{
    int choice;

    cout << "Menu:\n"
            "1: Sort by USD\n"
            "2: Sort by EURO\n"
            "3: Sort by BGN\n\n"
            "Other: EXIT\n\n"
            "Enter your choice: ";
    cin >> choice;

    return choice;
}

void print_depositors(const depositor depositors[], const int& depositors_count)
{
    cout << "----------------------------------------------------------------------\n";
    cout << "| " << setw(15) << left << "Name" << " |" << setw(15) << "USD" << " |" << setw(15) << "EURO" << " |" << setw(15) << "BGN" << " |\n";
    cout << "----------------------------------------------------------------------\n";
    cout << fixed << setprecision(2);
    for (int i = 0; i < depositors_count; i++)
    {
        cout << "| " << setw(15) << left << depositors[i].name << " |" << setw(15) << depositors[i].USD << " |" 
             << setw(15) << depositors[i].EURO << " |" << setw(15) << depositors[i].BGN << " |\n";
    }
    cout << "----------------------------------------------------------------------\n";
    
}

void create_depositor(depositor depositors[], const string& file_path, int& depositors_count)
{
    if (depositors_count < max_depositors)
    {
        ofstream file_stream(file_path, ofstream::out | ofstream::app);
        
        cout << "Enter Data for depositor #" << depositors_count + 1 << ":\n"
                "Name: ";
        cin.ignore();
        getline(cin, depositors[depositors_count].name);
        
        cout << "USD: ";
        cin >> depositors[depositors_count].USD;
        cout << "BLV: ";
        cin >> depositors[depositors_count].BGN;
        cout << "EURO: ";
        cin >> depositors[depositors_count].EURO;

        string parsed_name = depositors[depositors_count].name;
        replace(parsed_name.begin(), parsed_name.end(), ' ', '_');
        
        file_stream << parsed_name << " "
                    << depositors[depositors_count].USD << " "
                    << depositors[depositors_count].EURO << " "
                    << depositors[depositors_count].BGN << "\n";
        depositors_count++;

        file_stream.close();
    }
    else cout << "You can not add mode depositors. (" << max_depositors << "<< is maximum)";
}

void print_all_depositors(const depositor depositors[], const int& depositors_count)
{
    cout << "All Depositors:\n";
    print_depositors(depositors, depositors_count);
}

void search_by_name(const depositor depositors[])
{
    string name;

    depositor search_out[max_depositors];
    
    cout << "Enter name to search: " << endl;

    cin.ignore();
    getline(cin, name);

    int searched_depositors_count = 0;
    for (int i = 0; i < max_depositors; ++i)
    {
        if (depositors[i].name.find(name) != string::npos && !depositors[i].name.empty())
        {
            search_out[searched_depositors_count] = depositors[i];
            searched_depositors_count++;
        }
    }

    print_depositors(search_out, searched_depositors_count);
}

// ReSharper disable once CppInconsistentNaming
void print_all_in_BGN(const depositor depositors[])
{
    struct converted_depositor
    {
        string name;
        double money;
    };

    converted_depositor converted_depositors[max_depositors];
    for (int i = 0; i < max_depositors; ++i)
    {
        converted_depositors[i].name = depositors[i].name;
        converted_depositors[i].money = convert_all_to_BGN(depositors[i]);
    }
    
    int above;
    cout << "Enter minimal value (In BGN): ";
    cin >> above;

    converted_depositor filtered_depositors[max_depositors];

    int counter = 0;
    
    for (int i = 0; i < max_depositors; ++i)
    {
        if(converted_depositors[i].money >= above)
        {
            filtered_depositors[counter] = converted_depositors[i];
            counter++;
        }
    }

    cout << "------------------------------------\n";
    cout << "| " << setw(15) << left << "Name" << " |" << setw(15) << "BGN" << " |\n";
    cout << "------------------------------------\n";
    cout << fixed << setprecision(2);
    for (int i = 0; i < counter; i++)
    {
        cout << "| " << setw(15) << left << filtered_depositors[i].name << " |" << setw(15) << filtered_depositors[i].money << " |\n";
    }
    cout << "------------------------------------\n";
}

void sort_menu_handler(depositor depositors[], string& file_path, const int& depositors_count)
{
    const int choice = get_sort_menu_handle();

    switch (choice)
    {
    case 1: sort_depositors_by_currency(depositors, USD);
        break;
    case 2: sort_depositors_by_currency(depositors, EURO);
        break;
    case 3: sort_depositors_by_currency(depositors, BGN);
        break;
    default: return;
    }

    cout << flush;

    print_depositors(depositors, depositors_count);

    std::ofstream file_stream(file_path, std::ios::out | std::ios::trunc);

    for (int i = 0; i < depositors_count; ++i)
    {
        string parsed_name = depositors[i].name;
        replace(parsed_name.begin(), parsed_name.end(), ' ', '_');
        
        file_stream << parsed_name << " "
                    << depositors[i].USD << " "
                    << depositors[i].EURO << " "
                    << depositors[i].BGN << "\n";
    }

    file_stream.close();
}

/**
 * \brief 
 * \param depositors List of depositors from RAM or file
 * \param path datafile path
 * \param depositors_count real count of depositors
 * \return end code to work with main() function
 */
int main_menu_handler(depositor depositors[], string& path, int& depositors_count)
{
    wait_user_respond_and_clear();

    system("CLS");  // NOLINT(concurrency-mt-unsafe)
    
    const int choice = get_main_menu_choice();
    
    switch (choice)
    {
    case 1: create_depositor(depositors, path, depositors_count);
        break;
    case 2: print_depositors(depositors, depositors_count);
        break;
    case 3: search_by_name(depositors);
        break;
    case 4: print_all_in_BGN(depositors);
        break;
    case 5: sort_menu_handler(depositors, path, depositors_count);
        break;
    case 6: return exit_from_program;
    default: return continue_to_handle;
    }

    return continue_to_handle; 
}

#pragma endregion

int main(int argc, char* argv[])
{
    string path_to_datafile = "depositors_data.txt";
    int depositors_count(0);
    depositor depositors[max_depositors];

    load_depositors_from_file(depositors, path_to_datafile, depositors_count); //Database action

    //Menu Handler
    while (true)
        if (main_menu_handler(depositors, path_to_datafile, depositors_count) == 0)
            break;
    
    return 0;
}
