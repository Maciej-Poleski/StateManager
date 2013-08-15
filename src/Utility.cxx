#include "Utility.hxx"

#include <string>
#include <iterator>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "EventCategoryManager.hxx"

IdSet parseIdSetFile(const boost::filesystem::path& path)
{
    static const boost::regex regex("Identyfikatory:(\r?\n[[:digit:]]+)*(\r?\n)*",boost::regex_constants::extended);
    boost::filesystem::ifstream inputStream(path);
    std::string text;
    std::getline(inputStream,text,'\0');
    if(boost::regex_match(text,regex)==false)
        throw std::runtime_error("File "+path.string()+" is not correct ID set");
    IdSet result;
    static const boost::regex numberRegex("[[:digit:]]+",boost::regex_constants::extended);
    for(boost::sregex_iterator i(text.begin(),text.end(),numberRegex),e; i!=e; ++i)
    {
        result.push_back(boost::lexical_cast<IdSet::value_type>(i->str()));
    }
    return result;
}

boost::gregorian::date extractDateFromPath(const boost::filesystem::path& path)
{
    auto preparedPath=path;
    static const boost::regex regex(".*/([[:digit:]]+)/([[:digit:]]+)/([[:digit:]]+)",boost::regex_constants::extended);
    boost::smatch match;
    boost::regex_match(preparedPath.string(),match,regex);
    using boost::lexical_cast;
    return boost::gregorian::date(lexical_cast<uint_fast32_t>(match[1].str()),
                                  lexical_cast<uint_fast32_t>(match[2].str()),
                                  lexical_cast<uint_fast32_t>(match[3].str())
                                 );
}

std::pair<boost::regex,HandlerFunction>
getImplementationForEvent(const std::string& event,const std::string &filenameRegex)
{
    const boost::regex regex(filenameRegex,boost::regex_constants::extended);
    return {regex,[regex,event](const boost::filesystem::path &file) {
        boost::smatch match;
        boost::regex_match(file.filename().string(),match,regex);
        return FileSummary {match[1].str(),event,parseIdSetFile(file),
                            extractDateFromPath(file.parent_path())
                           };
    }
           };
}

std::pair< boost::regex, HandlerFunction >
getImplementationForInfixEvent(const std::string& event)
{
    return getImplementationForEvent(event,"(.+) "+event+".txt");
}

std::pair< boost::regex, HandlerFunction >
getImplementationForPrefixEvent(const std::string& event)
{
    return getImplementationForEvent(event,event+" (.+).txt");
}

DaySummary parseDay(const boost::filesystem::path& path,
                    const EventCategoryManager& categoryManager)
{
    DaySummary result;
    result.first=extractDateFromPath(path);
    using namespace boost::filesystem;
    for(auto i=directory_iterator(path); i!=directory_iterator(); ++i)
    {
        try
        {
            auto fileSummary= categoryManager.handleFile(*i);
            if(result.second.find(fileSummary.categoryName)==result.second.end())
                result.second[fileSummary.categoryName]=CategorySummary(categoryManager.eventComparator());
            result.second[fileSummary.categoryName][fileSummary.eventName]=fileSummary.idSet;
        }
        catch(std::runtime_error e)
        {
            std::clog<<"Unable to handle file: "<<e.what()<<'\n';
        }
    }
    return result;
}

GlobalState parseEventsForGlobalState(const boost::filesystem::path& eventsPath,
                                      const EventCategoryManager& categoryManager)
{
    GlobalState result;
    using namespace boost::filesystem;
    std::vector<path> years;
    std::copy(directory_iterator(eventsPath),directory_iterator(),std::back_inserter(years));
    std::sort(years.begin(),years.end());
    for(auto p : years)
        parseYearForGlobalState(p,categoryManager,result);
    return result;
}

void parseYearForGlobalState(const boost::filesystem::path& yearPath,
                             const EventCategoryManager& categoryManager,
                             GlobalState& result)
{
    using namespace boost::filesystem;
    std::vector<path> months;
    std::copy(directory_iterator(yearPath),directory_iterator(),std::back_inserter(months));
    std::sort(months.begin(),months.end());
    for(auto p : months)
        parseMonthForGlobalState(p,categoryManager,result);
}

void parseMonthForGlobalState(const boost::filesystem::path& monthPath,
                              const EventCategoryManager& categoryManager,
                              GlobalState& result)
{
    using namespace boost::filesystem;
    std::vector<path> days;
    std::copy(directory_iterator(monthPath),directory_iterator(),std::back_inserter(days));
    std::sort(days.begin(),days.end());
    for(auto p : days)
    {
        DaySummary day=parseDay(p,categoryManager);
        insertDaySummaryIntoGlobalState(result,day,categoryManager);
    }
}

void insertDaySummaryIntoGlobalState(GlobalState& globalState,
                                     const DaySummary& daySummary,
                                     const EventCategoryManager& categoryManager)
{
    for(auto i : daySummary.second)
    {
        auto category=i.first;
        insertCategorySummaryIntoCategoryState(globalState[category],
                                               i.second,
                                               categoryManager,
                                               daySummary.first
                                              );
    }
}

#include <boost/date_time/gregorian/gregorian.hpp>

void insertCategorySummaryIntoCategoryState(CategoryState& categoryState,
        const CategorySummary& categorySummary,
        const EventCategoryManager& categoryManager,
        const boost::gregorian::date &date)
{
    for(auto i : categorySummary)
    {
        if(categoryManager.isInitializationEvent(i.first))
        {
            for(auto j : i.second)
            {
                if(categoryState.find(j)!=categoryState.end())
                {
                    throw std::runtime_error("Reinitialization of "+
                                             std::to_string(j)+" as "+i.first);
                }
                categoryState[j]= {{date,i.first}};
            }
        }
        else
        {
            for(auto j : i.second)
            {
                if(!categoryManager.isAcceptableStateTransition())
                    throw std::runtime_error("Forbiden transition from "+" to ");
            }
        }
    }
}
