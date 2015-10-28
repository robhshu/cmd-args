#ifndef CMDARG_PARSER_H_
#define CMDARG_PARSER_H_

#include <string>

namespace cmdargs {
bool parse_storage(const std::string &raw, bool &res);
bool parse_storage(const std::string &raw, long long &res);
bool parse_storage(const std::string &raw, std::string &res);
};

#endif
