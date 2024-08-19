#ifndef _score_hpp_INCLUDED
#define _score_hpp_INCLUDED

#include "util.hpp"
#include <algorithm>
#include <stdio.h>

namespace CaDiCaL {

struct score_smaller {
  Internal *internal;
  score_smaller (Internal *i) : internal (i) {}
  bool operator() (unsigned a, unsigned b);
};

struct decision_group_score_smaller {
  Internal *internal;
  decision_group_score_smaller (Internal *i) : internal (i) {}
  bool operator() (unsigned a, unsigned b);
  unsigned get(unsigned v) const;
};

typedef heap<score_smaller> ScoreSchedule;

class GroupedScoreSchedule {
  private:
    std::vector<heap<score_smaller>> queues; // for each decision group, the usual EVSIDS heap
    heap<decision_group_score_smaller> dgs_heap; // heap of decision groups
    decision_group_score_smaller dg_sc;
    score_smaller sc;
    Internal *internal;

    heap<score_smaller>& heap_for(unsigned e) {
      unsigned queue_idx = dg_sc.get(e);
      return queues[queue_idx];
    }

    const heap<score_smaller>& heap_for_const(unsigned e) const {
      unsigned queue_idx = dg_sc.get(e);
      return queues[queue_idx];
    }
  public:

    GroupedScoreSchedule(Internal *internal) : dgs_heap(internal), dg_sc(internal), sc(internal) {
      queues.push_back(heap<score_smaller>(internal)); // initially, add a single heap for dg 0
    }

    void set_max_decision_group(size_t idx) {
      queues.resize(idx+1, heap<score_smaller>(sc));
    }

  // Number of elements
  size_t size () const {
    size_t sum = 0;
    for (auto &q : queues) {
      sum += q.size();
    }
    return sum;
  }

  // Check if no more elements are in the heap.
  //
  // Invariant: dgs_heap only contains non-empty heaps
  bool empty () const { return dgs_heap.empty(); }

  // Check whether 'e' is already in the heap.
  //
  bool contains (unsigned e) const {
    return heap_for_const(e).contains(e);
  }

  // Add a new (not contained) element 'e' to the heap.
  //
  void push_back (unsigned e) {
    assert (!contains (e));
    auto &heap = heap_for(e);
    heap.push_back(e);
    if (heap.size() == 1) {
      dgs_heap.push_back(dg_sc.get(e));
    }
  }

  // Returns the maximum element in the heap.
  //
  unsigned front () const {
    const heap<score_smaller> *front_heap = &queues[dgs_heap.front()];
    return front_heap->front();
  }

  int front_group () const {
    return dgs_heap.front();
  }

  // Removes the maximum element in the heap.
  //
  unsigned pop_front () {
    // fprintf(stderr, "pop_front %d size %d\n", dgs_heap.front(), dgs_heap.size());
    heap<score_smaller> *front_heap = &queues[dgs_heap.front()];
    auto retval = front_heap->pop_front();
    if (front_heap->size() == 0) {
      dgs_heap.pop_front();
    }
    return retval;
  }

  // Notify the heap, that evaluation of 'less' has changed for 'e'.
  //
  void update (unsigned e) {
    return heap_for(e).update(e);
  }

  void update_group_score (unsigned gp) {
    if (dgs_heap.contains(gp)) {
      fprintf(stderr, "update_group_score %d\n", gp);
      dgs_heap.update(gp);
    } else {
      fprintf(stderr, "update_group_score fail %d\n", gp);
    }
  }

  void pop_all_groups(vector<int> &dst) {
    while (!dgs_heap.empty())
      dst.push_back(dgs_heap.pop_front());
  }

  void insert_group_scores(const vector<int> &scores) {
    dgs_heap.clear();
    for (int gp : scores) {
      dgs_heap.push_back(gp);
    }
  }

  void clear () {
    dgs_heap.clear();
    for (auto &h : queues) h.clear();
  }

  void erase () {
    dgs_heap.erase();
    for (auto &h : queues) h.erase();
  }

  void shrink () {
    dgs_heap.shrink();
    for (auto &h : queues) h.shrink();
  }
};

} // namespace CaDiCaL

#endif
