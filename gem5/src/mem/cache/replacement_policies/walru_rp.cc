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
    // 與原始 Random 策略完全相同：標記為無效
    std::static_pointer_cast<WALRUReplData>(
        replacement_data)->valid = false;
}

void
WALRURP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // 與原始 Random 策略完全相同：不做任何事
    // 這是 Random 策略 activeEnergy 低的關鍵原因
}

void
WALRURP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    // 與原始 Random 策略完全相同：標記為有效
    std::static_pointer_cast<WALRUReplData>(
        replacement_data)->valid = true;
}

ReplaceableEntry*
WALRURP::getVictim(const ReplacementCandidates& candidates) const
{
    assert(candidates.size() > 0);

    // 階段 1：優先選擇無效項目（與原始 Random 相同）
    for (const auto& candidate : candidates) {
        if (!std::static_pointer_cast<WALRUReplData>(
                    candidate->replacementData)->valid) {
            return candidate;
        }
    }

    // 階段 2：極簡版乾淨優先邏輯
    // 預先隨機選擇一個候選項作為默認受害者
    ReplaceableEntry* victim = candidates[random_mt.random<unsigned>(0,
                                candidates.size() - 1)];
    
    // 極簡檢查：只隨機檢查最多 3 個候選項，找到乾淨的就立即返回
    // 這樣既有隨機性，又能傾向選擇乾淨塊，同時保持極低開銷
    const int MAX_CHECKS = 3;
    int checks = 0;
    
    for (int i = 0; i < MAX_CHECKS && i < candidates.size(); i++) {
        // 隨機選擇一個候選項（可能重複選擇，但這是有意的）
        int idx = random_mt.random<unsigned>(0, candidates.size() - 1);
        ReplaceableEntry* candidate = candidates[idx];
        CacheBlk* blk = static_cast<CacheBlk*>(candidate);
        
        checks++;
        
        // 如果找到乾淨塊，立即返回
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