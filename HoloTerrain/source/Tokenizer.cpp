#include "Tokenizer.h"
 
Tokenizer::Tokenizer ( const std::string& s, char delim )
{
  std::istringstream grabber ( s );
  std::string        token;
 
  while ( getline ( grabber, token, delim ) ) {
    if ( !token.empty() )
      tokens.push_back ( token );
  }
  current = tokens.begin();
}
 
Tokenizer::Tokenizer ( const std::string& s, const std::string& delim )
{
  std::string            token;
  std::string::size_type front = 0;
  std::string::size_type back = 0;
 
  while ( true ) {
    if ( back == std::string::npos )
      break;
    front = s.find_first_not_of ( delim, front );
    if ( front == std::string::npos )
      break;
    back = s.find_first_of ( delim, front );
    token = s.substr ( front, back - front );
    tokens.push_back ( token );
    front = back + delim.length();
  }
  current = tokens.begin();
}