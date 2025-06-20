#include "mem/cache/replacement_policies/walru_rp.hh"

#include <cassert>
#include <memory>

#include "params/WALRURP.hh"
#include "base/random.hh"
#include "mem/cache/blk.hh"

WALRURP::WALRURP(const Params *p)
    : BaseReplacementPolicy(p), writeCostWeight(p->write_cost_weight)
{
}

WALRURP::~WALRURP()
{
}

std::shared_ptr<ReplacementData>
WALRURP::instantiateEntry()
{
    return std::shared_ptr<ReplacementData>(new WALRUReplData());
}

void
WALRURP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
const
{
    //  Random 標記為無效
    std::static_pointer_cast<WALRUReplData>(
        replacement_data)->valid = false;
}

void
WALRURP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    //  Random 相同

}

void
WALRURP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    //  Random 相同
    std::static_pointer_cast<WALRUReplData>(
        replacement_data)->valid = true;
}

ReplaceableEntry*
WALRURP::getVictim(const ReplacementCandidates& candidates) const
{
    assert(candidates.size() > 0);

    //1 優先選擇無效 Random 相同
    for (const auto& candidate : candidates) {
        if (!std::static_pointer_cast<WALRUReplData>(
                    candidate->replacementData)->valid) {
            return candidate;
        }
    }

    // 2 乾淨優先
    // 先隨機選
    ReplaceableEntry* victim = candidates[random_mt.random<unsigned>(0,
                                candidates.size() - 1)];
    
    // 只隨機檢查 MAX_CHECKS 我用 3 個 找到乾淨立即返回
    const int MAX_CHECKS = 3;
    int checks = 0;
    
    for (int i = 0; i < MAX_CHECKS && i < candidates.size(); i++) {
        // 隨機選
        int idx = random_mt.random<unsigned>(0, candidates.size() - 1);
        ReplaceableEntry* candidate = candidates[idx];
        CacheBlk* blk = static_cast<CacheBlk*>(candidate);
        
        checks++;
        
        // 找到立即返回
        if (!blk->isDirty()) {
            return candidate;
        }
    }
    
    // 返回預先選好的隨機受害者
    return victim;
}

WALRURP*
WALRURPParams::create()
{
    return new WALRURP(this);
}