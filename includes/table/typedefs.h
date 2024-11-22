#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <vector>
#include <string>

#include "../bplustree/map.h"
#include "../bplustree/multimap.h"
#include "../token/child_tokens.h"

typedef std::vector<std::string> vectorstr;
typedef std::vector<long> vectorlong;

typedef Map<std::string, Token*> map_st;
typedef Map<std::string, long> map_sl;

typedef MMap<std::string, Token*> mmap_st;
typedef MMap<std::string, long> mmap_sl;
typedef MMap<std::string, std::string> mmap_ss;


#endif