#ifndef _score_hpp_INCLUDED
#define _score_hpp_INCLUDED

// #define NODECIDE_SCORE 1e150

// I think a negative score ensures these vars are lowest?
// #define NODECIDE_SCORE 1e10
#define NODECIDE_SCORE -1.0

namespace CaDiCaL {

struct score_smaller {
  Internal *internal;
  score_smaller (Internal *i) : internal (i) {}
  bool operator() (unsigned a, unsigned b);
};

typedef heap<score_smaller> ScoreSchedule;

} // namespace CaDiCaL

#endif
