#include <iostream>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <inc/ParseTable.h>
#include <inc/scanner.h>

using namespace std;

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    po::options_description desc("Options");

    // clang-format off
    desc.add_options()
        ("help,h", "Help message")
        ("file", po::value<string>(), "temp input lol")
        /*
        ("source-root,r", "Source root directory")
        ("entry,e", "Program entry point")
        */
        ;
    // clang-format on

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help"))
    {
        cout << desc << "\n";
        return 1;
    }

    // TODO print version and stuff

    if(vm.count("file") == 0)
    {
        cout << desc << "\n";
        return -1;
    }

    string filename = vm["file"].as<string>();

    if(!boost::filesystem::exists(filename))
    {
        cout << "Cannot find file '" << filename << "'\n";
        return -1;
    }

    sigil::Scanner scanner(filename);
    sigil::Symbol symbol;

    do
    {
        std::string tokenStr;
        symbol = scanner.nextToken(tokenStr);
        cout << sigil::TERMINAL_LOOKUP.at(symbol) << " ";
        if(symbol == sigil::Symbol::semicolon)
        {
            cout << "\n";
        }
    }
    while(symbol != sigil::Symbol::__EOF__);
}
