#ifndef _limit_hpp_INCLUDED
#define _limit_hpp_INCLUDED

namespace CaDiCaL {

struct Limit {

  long reduce;    // conflict limit for next 'reduce'
  long resolved;  // limit on keeping recently resolved clauses
  long restart;   // conflict limit for next 'restart'
  long subsume;   // next subsumption check

  int fixed;      // number of units in 'collect'
  int keptglue;   // maximum kept glue in 'reduce'
  int keptsize;   // maximum kept size in 'reduce'

  int decision_level_at_last_restart;
  long conflicts_at_last_restart;
  long conflicts_at_last_reduce;

  Limit ();
};

};

#endif
