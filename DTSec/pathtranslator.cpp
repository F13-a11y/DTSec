#include <string>
using namespace std;
string escapePath(const string& input) {
    string out;
    out.reserve(input.size() * 2); // tahmini kapasite
    for (char c : input) {
        if (c == '\\') {
            out += "\\\\"; // tek '\' yerine çift '\\'
        }
        else {
            out += c;
        }
    }
    return out;
}