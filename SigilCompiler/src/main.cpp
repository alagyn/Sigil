#include <fstream>
#include <iostream>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <hermes/parser.h>
#include <hermes/scanner.h>

#include <sigil/syntaxTree.h>

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

    auto input = std::make_shared<std::ifstream>(filename);
    auto scanner = hermes::Scanner::New(input);

    hermes::Parser parser;
    try
    {
        parser.parse(scanner);
    }
    catch(const std::runtime_error& err)
    {
        cout << err.what() << "\n";
    }
}
