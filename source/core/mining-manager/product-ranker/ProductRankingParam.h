/**
 * @file ProductRankingParam.h
 * @brief the parameters used in product ranking.
 * @author Jun Jiang <jun.jiang@izenesoft.com>
 * @date Created 2012-05-17
 */

#ifndef SF1R_PRODUCT_RANKING_PARAM_H
#define SF1R_PRODUCT_RANKING_PARAM_H

#include <common/inttypes.h>
#include <string>
#include <vector>
#include <mining-manager/group-manager/GroupParam.h>

namespace sf1r
{

struct ProductRankingParam
{
    const std::string& query_;

    std::vector<docid_t>& docIds_;

    std::vector<score_t>& relevanceScores_;

    const std::size_t docNum_;

    const faceted::GroupParam& groupParam_;

    category_id_t boostingCategoryId_;

    ProductRankingParam(
        const std::string& query,
        std::vector<docid_t>& docIds,
        std::vector<score_t>& relevanceScores,
        const faceted::GroupParam& groupParam
    )
        : query_(query)
        , docIds_(docIds)
        , relevanceScores_(relevanceScores)
        , docNum_(docIds.size())
        , groupParam_(groupParam)
        , boostingCategoryId_(0)
    {}

    bool isValid() const
    {
        return docNum_ == relevanceScores_.size();
    }
};

} // namespace sf1r

#endif // SF1R_PRODUCT_RANKING_PARAM_H