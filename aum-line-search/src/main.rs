#![feature(map_first_last)]

use std::cmp::Ordering;
use std::collections::btree_set::BTreeSet;

fn main() {
    let mut multiset: BTreeSet<IntersectionPoint> = BTreeSet::new();
    multiset.insert(IntersectionPoint {
        step_size: 100,
        number: 10,
        position: 10,
    });
    multiset.insert(IntersectionPoint {
        step_size: 100,
        number: 5,
        position: 6,
    });
    multiset.insert(IntersectionPoint {
        step_size: 200,
        number: 21,
        position: 34,
    });
    multiset.insert(IntersectionPoint {
        step_size: 200,
        number: 25,
        position: 46,
    });
    multiset.insert(IntersectionPoint {
        step_size: 100,
        number: 432,
        position: 123,
    });
    for x in multiset {
        dbg!(x);
    }
}

#[derive(Eq, PartialEq, PartialOrd, Debug)]
struct IntersectionPoint {
    step_size: u32,
    number: usize,
    position: usize,
}

impl IntersectionPoint {
    /*
    sub-routine modify_array: TODO check
    for i=0; i<self.number; i++: // reverse
      new_position = i + self.position ????
      old_position = self.position+self.number-1-i ????
      new_data[new_position] = Q[old_position]  ???
    for i=0; i<self.number; i++:
      position = i + self.position
      Q[position] = new_data[position]
    if self.position > 0: // maybe add at beginning
      S.maybe add intersection point(self.position-1)
    if self.position < B-1: //maybe add at end
     S.maybe add intersection point(self.position)
     */
    fn modify_array(&mut self, q: &mut Vec<u32>, b: usize) {
        let mut new_data = Vec::with_capacity(q.len());
        for i in 0..self.number {
            let new_position = i + self.position;
            let old_position = self.position + self.number - 1 - i;
            new_data[new_position] = q[old_position];
        }
        for i in 0..self.number {
            let position = i + self.position;
            q[position] = new_data[position];
        }

        // maybe add at beginning
        if self.position > 0 {
            // S.maybe add intersection point(self.position-1)
        }

        //maybe add at end
        if self.position < b - 1 {
            // S.maybe add intersection point(self.position)
        }
    }
}

impl Ord for IntersectionPoint {
    fn cmp(&self, other: &Self) -> Ordering {
        let step_size_ordering = self.step_size.cmp(&other.step_size);
        if step_size_ordering == Ordering::Equal {
            if self.number != other.number || self.position != other.position {
                // this ensures partial equality
                return Ordering::Greater;
            }
        }
        step_size_ordering
    }
}

fn intersection_point(i: usize) -> IntersectionPoint { todo!() }

fn t_b(i: usize) -> u32 { todo!() }

fn main_algorithm(b: usize) {
    // initialize array Q based on t_b(0) values, each element is one of 0, ..., B-1.
    let mut Q = Vec::with_capacity(b);
    for i in 0..(b - 1) {
        Q[i] = t_b(i)
    }
    // Algorithm keeps Q such that T_Q[i](s) < T_Q[i+1](s).

    // initialize empty multiset S, iteration=0.
    // for all i from 1 to B-1:
    //   S.maybe add intersection point(i)
    let mut S: BTreeSet<IntersectionPoint> = BTreeSet::new();
    for i in 1..(b - 1) {
        // maybe add??
        S.insert(intersection_point(i));
    }

    // while S is not empty and iteration++ < max_iterations:
    //   step_size = S.begin().step_size
    //   while S.begin().step_size == step_size: // for ties
    //     intersection = S.pop()
    //     intersection.modify_array(Q) //TODO and FP/FN matrices
    let max_iterations = 500;
    let mut iteration = 0;
    while !S.is_empty() && iteration < max_iterations {
        iteration += 1;
        let first_inter = if let Some(first_inter) = S.first() {
            first_inter
        } else { todo!() };
        let step_size = first_inter.step_size;

        while let Some(mut intersection) = S.pop_first() {
            intersection.modify_array(&mut Q, b);
        }
    }
}
