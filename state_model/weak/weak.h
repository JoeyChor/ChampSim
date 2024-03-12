#ifndef STATE_MODEL_WEAK_H
#define STATE_MODEL_WEAK_H

#include <vector>

#include "access_type.h"
#include "cache.h"
#include "modules.h"


struct weak : champsim::modules::state_model {
  enum class STATE { MODIFIED, WAITING_ACK, WAITING_FILL, INVALID };
  long NUM_SET, NUM_WAY;
  std::vector<STATE> cache_state;
  uint64_t cycle = 0;

  explicit weak(CACHE* cache);
  weak(CACHE* cache, long sets, long ways);
  bool handle_pkt(champsim::address address, champsim::address ip, access_type type, uint32_t cpu);
  bool handle_response(champsim::channel::response_type resp);
  void initialize_state_model();
  void final_stats();
};

#endif
