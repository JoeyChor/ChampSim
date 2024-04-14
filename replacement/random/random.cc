#include "random.h"

random::random(CACHE* cache) : random(cache, cache->NUM_WAY) {}

random::random(CACHE* cache, long ways) : replacement(cache), dist(0, ways - 1),
                                          log(cache->NAME == "cpu0_L1D" ? std::make_optional(fmt::output_file("random.log")) : std::nullopt)
{
}
long random::find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set, const champsim::cache_block* current_set, champsim::address ip,
                         champsim::address full_addr, access_type type)
{
  return dist(rng);
}

void random::replacement_cache_fill(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip,
                                    champsim::address victim_addr, access_type type)
{
}
void random::update_replacement_state(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip,
                                      champsim::address victim_addr, access_type type, uint8_t hit)
{
  if (log.has_value())
    log->print("{}\n", full_addr);
}