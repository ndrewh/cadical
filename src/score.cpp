#include "internal.hpp"

namespace CaDiCaL {

// This initializes variables on the binary 'scores' heap also with
// smallest variable index first (thus picked first) and larger indices at
// the end.
//
void Internal::init_scores (int old_max_var, int new_max_var) {
  LOG ("initializing EVSIDS scores from %d to %d", old_max_var + 1,
       new_max_var);
  for (int i = old_max_var; i < new_max_var; i++)
    scores.push_back (i + 1);

  if (dgstab.size() == 0)
    dgstab.resize(1, 0);
}

// Shuffle the EVSIDS heap.

void Internal::shuffle_scores () {
  if (opts.decisiongroupshuffle) {
    assert (false);
    // fprintf(stderr, "decisiongroupshuffle\n");
    // vector<int> shuffle;
    // scores.pop_all_groups(shuffle);
    // Random random (opts.seed); // global seed
    // random += stats.shuffled;  // different every time
    // for (size_t i = 0; i < shuffle.size(); i++) {
    //   const int j = random.pick_int (i, shuffle.size()-1);
    //   swap (shuffle[i], shuffle[j]);
    // }

    // int tmp = 0;
    // for (auto &idx : shuffle) {
    //   if (dgstab[idx] <= max_dgroup) // don't reassign manually set scores
    //     dgstab[idx] = tmp++;
    // }
    // scores.insert_group_scores(shuffle);
    // stats.shuffled++;
  }

  if (!opts.shuffle)
    return;
  if (!opts.shufflescores)
    return;
  assert (!level);
  stats.shuffled++;
  LOG ("shuffling scores");
  vector<int> shuffle;
  if (opts.shufflerandom) {
    scores.erase ();
    for (int idx = max_var; idx; idx--)
      shuffle.push_back (idx);
    Random random (opts.seed); // global seed
    random += stats.shuffled;  // different every time
    for (int i = 0; i <= max_var - 2; i++) {
      const int j = random.pick_int (i, max_var - 1);
      swap (shuffle[i], shuffle[j]);
    }
  } else {
    while (!scores.empty ()) {
      int idx = scores.front ();
      (void) scores.pop_front ();
      shuffle.push_back (idx);
    }
  }
  score_inc = 0;
  for (const auto &idx : shuffle) {
    stab[idx] = score_inc++;
    scores.push_back (idx);
  }
}

} // namespace CaDiCaL
