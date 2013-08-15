
#ifndef TYPES_HXX
#define TYPES_HXX

#include <vector>
#include <map>
#include <string>
#include <functional>

#include <boost/filesystem/path.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>

typedef std::size_t IdType;

typedef std::vector<IdType> IdSet;

typedef std::map<std::string,IdSet,std::function<bool(const std::string&,const std::string&)>> CategorySummary;
//               ^ event
typedef std::pair<boost::gregorian::date,std::map<std::string,CategorySummary>> DaySummary;

struct FileSummary
{
    std::string categoryName;
    std::string eventName;
    IdSet idSet;
    boost::gregorian::date date;
};

typedef std::function<FileSummary(const boost::filesystem::path &)> HandlerFunction;

typedef std::string State;

typedef std::vector<std::pair<boost::gregorian::date,State>> StateHistory;

typedef std::map<IdType,StateHistory> CategoryState;

typedef std::map<std::string,CategoryState> GlobalState;

#endif // TYPES_HXX
