///////////////////////////////////////////////////////////////////////////////
// gnomes_algs.hpp
//
// Algorithms that solve the greedy gnomes problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on gnomes_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>

#include "gnomes_types.hpp"

#include <math.h>

#include <vector>

namespace gnomes {

// Solve the greedy gnomes problem for the given grid, using an exhaustive
// search algorithm.
//
// This algorithm is expected to run in exponential time, so the grid's
// width+height must be small enough to fit in a 64-bit int; this is enforced
// with an assertion.
//
// The grid must be non-empty.
path greedy_gnomes_exhaustive(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  // Compute maximum path length, and check that it is legal.
  const size_t max_steps = setting.rows() + setting.columns() - 2;
  assert(max_steps < 64);

  // TODO: implement the exhaustive search algorithm, then delete this
  // comment.
  path best(setting);
  int bit;

  for(int len = 0; len <= max_steps; len++){
    for(int bits = 0; bits <= pow(2, len) - 1; bits++){
      path candidate(setting);
      for(int k = 0; k <= len - 1; k++){
        bit = (bits >> k) & 1;
        if(bit == 1){
          if(candidate.is_step_valid(STEP_DIRECTION_RIGHT)){
            candidate.add_step(STEP_DIRECTION_RIGHT);
          }
        }
        else{
          if(candidate.is_step_valid(STEP_DIRECTION_DOWN)){
            candidate.add_step(STEP_DIRECTION_DOWN);
          }
        }
      }
      if(best.total_gold() == 0 || candidate.total_gold() > best.total_gold()){
        best = candidate;
      }
    }
  }

  return best;
}

// Solve the greedy gnomes problem for the given grid, using a dynamic
// programming algorithm.
//
// The grid must be non-empty.
path greedy_gnomes_dyn_prog(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  path best(setting);
  const size_t R = setting.rows();
  const size_t C = setting.columns();
  bool none = false;


  std::vector<std::vector<path>> A(R);



  for(size_t i = 0; i < R; i++){
    for (size_t j = 0; j < C; j++){
      A[i].push_back(path(setting));
    }
  }


  for(int i = 0; i <= setting.rows() - 1; i++){

    for (int j = 0; j <= setting.columns() - 1; j++){


      if(setting.get(i, j) == 'X'){
        A[i][j] = path(setting);
        none = true;
        continue;
      }

      path from_left = path(setting);
      path from_above = path(setting);
      bool left_none = true;
      bool above_none = true;


      std::vector<step> temp_steps = A[i-1][j].steps();
      if(i > 0 && temp_steps.size() == 1){

        from_above = A[i-1][j];
        if(from_above.is_step_valid(STEP_DIRECTION_DOWN)){
          from_above.add_step(STEP_DIRECTION_DOWN);
          above_none = false;

        }
      }
      temp_steps = A[i][j-1].steps();
      if(j > 0 && temp_steps.size() == 1){
        from_left = A[i][j-1];
        if(from_left.is_step_valid(STEP_DIRECTION_RIGHT)){
          from_left.add_step(STEP_DIRECTION_RIGHT);
          left_none = false;
        }
      }

      if(!left_none && !above_none){
        if(from_left.total_gold() > from_above.total_gold()){
          A[i][j] = from_left;

        }
        else{
          A[i][j] = from_above;
        }
      }
      else if(!left_none && above_none){
        A[i][j] = from_left;
      }
      else if(left_none && !above_none){
        A[i][j] = from_above;
      }
      else if(left_none && above_none && i != 0){
        A[i][j] = path(setting);
        none = true;
      }
      A[i][j].print();
    }
  }

  for (int i = 0; i <= setting.rows() -1; i++){
    for (int j = 0; j <= setting.columns() -1; j++){
      if (A[i][j].total_gold() > best.total_gold()){
        best = A[i][j];
      }
    }
  }


  return best;
}

}
