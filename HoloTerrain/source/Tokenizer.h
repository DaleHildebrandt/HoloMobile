#include <sstream>
#include <string>
#include <vector>
 
class Tokenizer {
public:
  explicit Tokenizer ( const std::string& s, char delim = ' ' );
  explicit Tokenizer ( const std::string& s, const std::string& delim );
public:
  std::string next() { return !done() ? *current++ : std::string(); }
  bool done() const { return current == tokens.end(); }
private:
  std::vector<std::string>           tokens;
  std::vector<std::string>::iterator current;
};