#include "converter.hpp"
#include "utils.hpp"

int main(const int argc, const char *argv[])
{
    std::string filename = get_file_name_from_argv(argc, argv);
    Converter converter(filename);
    return EXIT_SUCCESS;
}