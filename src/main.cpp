#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <uuid/uuid.h>

#define UUID_BYTES 37
#define DATE_START 1590000000000 // 2020-05-20
#define DATE_END 1608744323000   // 2020-12-23

#define HEADERS "-h"
#define DATA_TYPES "-d"
#define ROW_COUNT "-rc"
#define FILE_NAME "-n"
#define PREFIX '-'

#define NUMBER "number"
#define DATE "date"
#define STRING "string"

static const char *REQUIRED_ARGS[3] = {HEADERS, DATA_TYPES, ROW_COUNT};
static const std::set<std::string> VALID_TYPES = {"date", "number", "string"};

typedef std::map<std::string, std::vector<std::string>> CommandMap;

void print_usage();
void print_missing_args(CommandMap *command_map);
void write_csv(std::string *file_name, std::vector<std::string> *headers, std::vector<std::string> *data_types, size_t row_count);
bool validate_types(std::vector<std::string> *data_types);
std::string get_uuid();
std::string to_date_string(long long ms);
std::string get_random_date(long max, long min);
CommandMap create_command_map(int argc, char **argv);

int main(int argc, char **argv)
{
    size_t row_count;
    std::string file_name;
    std::vector<std::string> headers, data_types;
    CommandMap command_map = create_command_map(argc, argv);

    try
    {
        headers = command_map.at(HEADERS);
        data_types = command_map.at(DATA_TYPES);
        row_count = std::stoll(command_map.at(ROW_COUNT).at(0));
    }
    catch (std::out_of_range e)
    {
        print_missing_args(&command_map);
        print_usage();
        exit(1);
    }

    if (!validate_types(&data_types))
    {
        std::cout << "Invalid data types" << std::endl;
        print_usage();
        exit(1);
    }

    if (command_map.find(FILE_NAME) != command_map.end())
        file_name = command_map.at(FILE_NAME).at(0) + ".csv";
    else
        file_name = get_uuid() + ".csv";

    write_csv(&file_name, &headers, &data_types, row_count);
}

bool validate_types(std::vector<std::string> *data_types)
{
    bool is_valid = true;

    for (std::string data_type : *data_types)
    {
        if (VALID_TYPES.find(data_type) == VALID_TYPES.end())
        {
            is_valid = false;
            break;
        }
    }
    return is_valid;
}

void write_csv(std::string *file_name, std::vector<std::string> *headers, std::vector<std::string> *data_types, size_t row_count)
{
    std::ofstream csv_file;
    csv_file.open(*file_name);
    size_t column_count = headers->size();
    size_t k = 0;
    size_t j = 0;

    for (size_t i = 0; i < column_count; ++i)
        if (i == column_count - 1)
            csv_file << headers->at(i) << '\n';
        else
            csv_file << headers->at(i) << ",";

    while ((++j) <= row_count)
    {
        k = 0;
        while (k < column_count)
        {
            std::string data_type = data_types->at(k);
            if (data_type == DATE)
                csv_file << get_random_date(DATE_START, DATE_END);
            else if (data_type == STRING)
                csv_file << get_uuid();
            else
                csv_file << j;

            if (k == (column_count - 1))
                csv_file << '\n';
            else
                csv_file << ',';
            ++k;
        }
    }

    csv_file.close();
}

CommandMap create_command_map(int argc, char **argv)
{
    CommandMap command_map;

    for (int i = 1; i < argc; ++i)
    {
        std::string argv_str = std::string(argv[i]);
        if (argv_str.at(0) == PREFIX)
        {
            std::vector<std::string> args;
            for (int j = i + 1; j < argc; ++j)
            {
                std::string next_arg = std::string(argv[j]);
                if (next_arg.at(0) == PREFIX || next_arg == "")
                    break;
                args.push_back(next_arg);
            }
            command_map[argv_str] = args;
        }
    }

    return command_map;
}

void print_missing_args(CommandMap *command_map)
{
    for (auto arg : REQUIRED_ARGS)
        if (command_map->find(arg) == command_map->end())
            std::cout << "Missing : " << arg << " is required " << std::endl;
}

void print_usage()
{
    std::cout << '\n'
              << "CSV Generator usage  📂: \n"
              << "Required: \n"
              << "-h specify headers ie foo bar baz \n"
              << "-d specify data types corresponding to header valid types: number string date \n"
              << "-rc specify row count i.e 100000 \n"
              << '\n'
              << "Optional: \n"
              << "-n file name, csv is suffixed by programme if not specified a uuid will be assigned \n";
}

std::string get_random_date(long max, long min)
{
    return to_date_string(rand() % (max - min + 1) + min);
}

std::string to_date_string(long long ms)
{
    auto date_ms = std::chrono::milliseconds(ms);
    auto time_point = std::chrono::time_point<std::chrono::system_clock>(date_ms);
    auto in_time_t = std::chrono::system_clock::to_time_t(time_point);
    auto local_time = std::localtime(&in_time_t);

    std::stringstream ss;
    ss << std::put_time(local_time, "%Y-%m-%d");
    return ss.str();
}

std::string get_uuid()
{
    uuid_t binuuid;
    char uuid[37];

    uuid_generate_random(binuuid);
    uuid_unparse_lower(binuuid, uuid);

    return std::string(uuid);
}
