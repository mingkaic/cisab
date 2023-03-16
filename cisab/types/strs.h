
#ifndef CISAB_TYPES_STRS_H
#define CISAB_TYPES_STRS_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace cisab
{

namespace types
{

using StringsT = std::vector<std::string>;

using StrSetT = std::set<std::string>;

using StrUSetT = std::unordered_set<std::string>;

template <typename VAL>
using StrMapT = std::map<std::string,VAL>;

template <typename VAL>
using StrUMapT = std::unordered_map<std::string,VAL>;

}

}

#endif // CISAB_TYPES_STRS_H
