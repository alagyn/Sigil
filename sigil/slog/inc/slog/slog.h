#include <string>

namespace sigil {

enum class LogLevel
{
    Trace,
    Debug,
    Info,
    Warn,
    Error
};

class SLog
{
public:
    static void log(const std::string& msg);
    static void dbg(const std::string& msg);

    static void setLevel(LogLevel level);
};

} //namespace sigil