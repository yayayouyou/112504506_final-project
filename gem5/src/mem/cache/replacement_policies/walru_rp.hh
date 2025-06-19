/**
 * @file
 * Declaration of an Ultra-Simple Clean-Aware Random Policy (CARP).
 * Minimalist approach to optimize PCM energy with near-zero overhead.
 */

#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_WALRU_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_WALRU_RP_HH__

#include "mem/cache/replacement_policies/base.hh"

struct WALRURPParams;

class WALRURP : public BaseReplacementPolicy
{
  protected:
    /** Ultra-Simple implementation of replacement data. */
    struct WALRUReplData : ReplacementData
    {
        /**
         * Flag informing if the replacement data is valid or not.
         * Invalid entries are prioritized to be evicted.
         */
        bool valid;

        /**
         * Default constructor. Invalidate data.
         */
        WALRUReplData() : valid(false) {}
    };

    /** Policy configuration parameter (unused in this simple version) */
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
     * Invalidate replacement data to set it as the next probable victim.
     */
    void invalidate(const std::shared_ptr<ReplacementData>& replacement_data)
                                                              const override;

    /**
     * Touch an entry to update its replacement data.
     * Deliberately empty to minimize overhead.
     */
    void touch(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Reset replacement data for new entry.
     */
    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;

    /**
     * Find replacement victim using Ultra-Simple Clean-Aware Random logic.
     * Uses limited sampling to favor clean blocks without full sorting overhead.
     */
    ReplaceableEntry* getVictim(const ReplacementCandidates& candidates) const
                                                                     override;

    /**
     * Instantiate a replacement data entry.
     */
    std::shared_ptr<ReplacementData> instantiateEntry() override;
};

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_WALRU_RP_HH__