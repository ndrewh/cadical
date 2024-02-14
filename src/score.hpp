#ifndef _score_hpp_INCLUDED
#define _score_hpp_INCLUDED

// #define NODECIDE_SCORE 1e150
#define NODECIDE_SCORE 0.0

namespace CaDiCaL {

struct score_smaller {
  Internal *internal;
  score_smaller (Internal *i) : internal (i) {}
  bool operator() (unsigned a, unsigned b);
};

typedef heap<score_smaller> ScoreSchedule;

} // namespace CaDiCaL

#endif
