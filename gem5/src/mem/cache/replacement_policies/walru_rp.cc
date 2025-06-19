#include "mem/cache/replacement_policies/walru_rp.hh"

#include <cassert>
#include <memory>

#include "params/WALRURP.hh"
#include "sim/core.hh"
#include "mem/cache/blk.hh"  // 新增：用於存取 CacheBlk

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
    return std::shared_ptr<ReplacementData>(new WALRUReplData);
}

void
WALRURP::invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
const
{
    std::shared_ptr<WALRUReplData> walru_repl_data =
        std::static_pointer_cast<WALRUReplData>(replacement_data);
    
    walru_repl_data->lastTouchTick = Tick(0);
    walru_repl_data->accessCount = 0;
    walru_repl_data->writeCount = 0;
}

void
WALRURP::touch(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    std::shared_ptr<WALRUReplData> walru_repl_data =
        std::static_pointer_cast<WALRUReplData>(replacement_data);
    
    walru_repl_data->lastTouchTick = curTick();
    walru_repl_data->accessCount++;
    walru_repl_data->writeCount++;  // 簡化：假設每次 touch 都可能寫入
}

void
WALRURP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    std::shared_ptr<WALRUReplData> walru_repl_data =
        std::static_pointer_cast<WALRUReplData>(replacement_data);
    
    walru_repl_data->lastTouchTick = curTick();
    walru_repl_data->accessCount = 1;
    walru_repl_data->writeCount = 0;
}

ReplaceableEntry*
WALRURP::getVictim(const ReplacementCandidates& candidates) const
{
    assert(candidates.size() > 0);
    
    // **策略：Clean-First Policy**
    // Phase 1: 優先尋找乾淨的塊
    ReplaceableEntry* clean_victim = nullptr;
    Tick oldest_clean_time = curTick();
    
    for (const auto& candidate : candidates) {
        // 將 ReplaceableEntry 轉型為 CacheBlk 以存取 isDirty()
        CacheBlk* blk = static_cast<CacheBlk*>(candidate);
        
        if (!blk->isDirty()) {  // 乾淨塊
            std::shared_ptr<WALRUReplData> data =
                std::static_pointer_cast<WALRUReplData>(candidate->replacementData);
            
            if (data->lastTouchTick < oldest_clean_time || clean_victim == nullptr) {
                clean_victim = candidate;
                oldest_clean_time = data->lastTouchTick;
            }
        }
    }
    
    // 如果找到乾淨塊，直接返回
    if (clean_victim) {
        return clean_victim;
    }
    
    // Phase 2: 如果都是髒塊，選擇寫入次數最少的
    ReplaceableEntry* victim = candidates[0];
    std::shared_ptr<WALRUReplData> victim_data =
        std::static_pointer_cast<WALRUReplData>(victim->replacementData);
    unsigned min_writes = victim_data->writeCount;
    Tick oldest_time = victim_data->lastTouchTick;
    
    for (const auto& candidate : candidates) {
        std::shared_ptr<WALRUReplData> candidate_data =
            std::static_pointer_cast<WALRUReplData>(candidate->replacementData);
        
        // 選寫入最少的，如果相同則選最老的
        if (candidate_data->writeCount < min_writes ||
            (candidate_data->writeCount == min_writes && 
             candidate_data->lastTouchTick < oldest_time)) {
            victim = candidate;
            min_writes = candidate_data->writeCount;
            oldest_time = candidate_data->lastTouchTick;
        }
    }
    
    return victim;
}

WALRURP*
WALRURPParams::create()
{
    return new WALRURP(this);
}