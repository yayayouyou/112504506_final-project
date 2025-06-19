/**
 * @file
 * Declaration of a Clean-First Write-Aware LRU replacement policy for PCM energy optimization.
 * Prioritizes clean blocks over dirty blocks to reduce PCM write energy.
 */

#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_WALRU_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_WALRU_RP_HH__

#include "mem/cache/replacement_policies/base.hh"

struct WALRURPParams;

class WALRURP : public BaseReplacementPolicy
{
  protected:
    /** WALRU-specific implementation of replacement data. */
    struct WALRUReplData : ReplacementData
    {
        /** Tick on which the entry was last touched. */
        Tick lastTouchTick;
        
        /** Number of times this block was accessed. */
        unsigned accessCount;
        
        /** Number of write operations to this block. */
        unsigned writeCount;

        /**
         * Default constructor. Initialize data.
         */
        WALRUReplData() : lastTouchTick(0), accessCount(0), writeCount(0) {}
    };

    /** Weight factor for write cost in victim selection. */
    const double writeCostWeight;

  public:
    /** Convenience typedef. */
    typedef WALRURPParams Params;

    /**
     * Construct and initialize this replacement policy.
     */
    WALRURP(const Params *p);

    /**
     * Destructor.
     */
    ~WALRURP();

    /**
     * Invalidate replacement data.
     */
    void invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
                                                              const override;

    /**
     * Touch an entry to update its replacement data.
     */
    void touch(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Reset replacement data for new entry.
     */
    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Find replacement victim using Clean-First strategy.
     * Prioritizes clean blocks over dirty blocks to reduce PCM writes.
     */
    ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const
                                                                     override;

    /**
     * Instantiate a replacement data entry.
     */
    std::shared_ptr<ReplacementData> instantiateEntry() override;
};

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_WALRU_RP_HH__