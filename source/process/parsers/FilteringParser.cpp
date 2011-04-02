/**
 * @file core/common/parsers/FilteringParser.cpp
 * @author Ian Yang
 * @date Created <2010-06-11 17:31:38>
 */
#include "FilteringParser.h"
#include <common/parsers/ConditionArrayParser.h>

#include <common/IndexBundleSchemaHelpers.h>
#include <common/ValueConverter.h>

namespace sf1r {

bool FilteringParser::parse(const Value& conditions)
{
    clearMessages();

    ConditionArrayParser conditionsParser;
    if (!conditionsParser.parse(conditions))
    {
        error() = conditionsParser.errorMessage();
        return false;
    }

    std::size_t conditionCount = conditionsParser.parsedConditionCount();
    filteringRules_.resize(conditionCount);
    for (std::size_t i = 0; i < conditionCount; ++i)
    {
        // create two alias
        const ConditionParser& condition =
            conditionsParser.parsedConditions(i);
        QueryFiltering::FilteringType& filteringRule = filteringRules_[i];

        // validation
        if (!isPropertyFilterable(indexSchema_, condition.property()))
        {
            error() = "Property is not filterable in condition: " +
                      condition.property();
            return false;
        }

        sf1r::PropertyDataType dataType =
            getPropertyDataType(indexSchema_, condition.property());
        if (dataType == sf1r::UNKNOWN_DATA_PROPERTY_TYPE)
        {
            error() = "Property's data type is unknown: " +
                      condition.property();
            return false;
        }

        filteringRule.first.second = condition.property();
        filteringRule.first.first = toFilteringOperation(condition.op());

        filteringRule.second.resize(condition.size());
        for (std::size_t v = 0; v < condition.size(); ++v)
        {
            ValueConverter::driverValue2PropertyValue(
                dataType,
                condition(v),
                filteringRule.second[v]
            );
        }
    }

    return true;
}

QueryFiltering::FilteringOperation FilteringParser::toFilteringOperation(
    const std::string& op
)
{
    if (op == "=")
    {
        return QueryFiltering::EQUAL;
    }
    else if (op == "<>")
    {
        return QueryFiltering::NOT_EQUAL;
    }
    else if (op == "in")
    {
        return QueryFiltering::INCLUDE;
    }
    else if (op == ">")
    {
        return QueryFiltering::GREATER_THAN;
    }
    else if (op == ">=")
    {
        return QueryFiltering::GREATER_THAN_EQUAL;
    }
    else if (op == "<")
    {
        return QueryFiltering::LESS_THAN;
    }
    else if (op == "<=")
    {
        return QueryFiltering::LESS_THAN_EQUAL;
    }
    else if (op == "between")
    {
        return QueryFiltering::RANGE;
    }
    else if (op == "starts_with")
    {
        return QueryFiltering::PREFIX;
    }
    else if (op == "ends_with")
    {
        return QueryFiltering::SUFFIX;
    }
    else if (op == "contains")
    {
        return QueryFiltering::SUB_STRING;
    }
    else if (op == "not_in")
    {
        return QueryFiltering::EXCLUDE;
    }

    static const bool kNeverGoesHere = false;
    BOOST_ASSERT(kNeverGoesHere);

    return QueryFiltering::NULL_OPERATOR;
}


} // namespace sf1r
