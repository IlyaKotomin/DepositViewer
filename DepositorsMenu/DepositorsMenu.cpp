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
            "6. EXIT\n\n"
            "Enter your choice: ";
    cin >> choice;

    return choice;
}

void create_depositor(depositor depositors[], ofstream& file_stream, int& depositors_count)
{
    if (depositors_count < max_depositors)
    {
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
    }
    else cout << "You can not add mode depositors. (50 is maximum)";
}

void print_depositors(const depositor depositors[], const int& depositors_count)
{
    cout << "All Depositors:\n";
    cout << setw(20) << "Name" << setw(10) << "USD" << setw(10) << "EURO" << setw(10) << "BGN\n";
    for (int i = 0; i < depositors_count; i++)
    {
        cout << setw(20) << depositors[i].name
             << setw(10) << depositors[i].USD
             << setw(10) << depositors[i].EURO 
             << setw(10) << depositors[i].BGN << "\n";
    }
}

void search_by_name(const depositor depositors[]){}

void print_above_set_amount(const depositor depositors[]){}

void handle_sort_menu(depositor depositors[], ofstream& file, int& depositors_count){}

int main_menu_handler(depositor depositors[], ofstream& file, int& depositors_count)
{
    const int choice = get_main_menu_choice();

    switch (choice)
    {
    case 1: create_depositor(depositors, file, depositors_count);
        break;
    case 2: print_depositors(depositors, depositors_count);
        break;
    case 3: search_by_name(depositors);
        break;
    case 4: print_above_set_amount(depositors);
        break;
    case 5: handle_sort_menu(depositors, file, depositors_count);
        break;
    case 6: return exit_from_program;
    default: break;
    }

    wait_user_respond_and_clear();
    
    return continue_to_handle; 
}

#pragma endregion


int main(int argc, char* argv[])
{
    const string data_file = "depositors_data.txt";

    depositor depositors[max_depositors];
    int depositors_count(0);
    load_depositors_from_file(depositors, data_file, depositors_count);

    ofstream out_file_stream(data_file, ofstream::app);
    
    while (true)
        if (main_menu_handler(depositors, out_file_stream, depositors_count) == 0)
            break;

    out_file_stream.close();
    
    return 0;
}
