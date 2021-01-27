use crate::proc::*;
use std::{cmp, collections::HashSet};
pub struct PriorityRRScheduler {
    procs: Vec<Proc>,
}
const TIME_QUANTUM: u32 = 10;

impl PriorityRRScheduler {
    
    pub fn new() -> PriorityRRScheduler {
        PriorityRRScheduler {
            procs: Vec::new()
        }
    }

    pub fn add(&mut self, t: Proc) {
        self.procs.push(t);
    }

    fn get(&self, i: usize) -> &Proc {
        &self.procs[i]
    }

    fn schedule_proc(&mut self, idx: usize, duration: u32) {
        &self.procs[idx].schedule_for(duration);
    }

    fn schedule_equal_priority(&mut self, v: &mut Vec<usize>) {
        let mut hs: HashSet<usize> = HashSet::new();
        loop {
            for i in 0..v.len() {
                let i = v[i];
                if hs.contains(&i) {
                    continue;
                }
                if (self.procs[i]).get_rem() > TIME_QUANTUM {
                    (self.procs[i]).schedule_for(TIME_QUANTUM);
                    println!("Scheduled {} for {}", (self.procs[i]).get_name(), TIME_QUANTUM);
                } else {
                    let schedule_time: u32 = cmp::min(TIME_QUANTUM, (self.procs[i]).get_rem());
                    (self.procs[i]).schedule_for(schedule_time);
                    println!("Scheduled {} for {} (finished)", (self.procs[i]).get_name(), schedule_time);
                    hs.insert(i);
                }
            }

            if v.iter().all(|x| hs.contains(x)) {
                break;
            }
        }

        self.remove_bulk(v);
    }

    fn remove_bulk(&mut self, v: &mut Vec<usize>) {
        v.reverse();
        for i in v {
            self.remove(*i);
        }
    }

    pub fn operate(&mut self) {
        let mut highest_p = self.highest_priority();
        while highest_p != std::u32::MAX {
            let mut v = self.highest_priority_ids();
            self.schedule_equal_priority(&mut v);

            highest_p = self.highest_priority();
        }
    }

    fn remove(&mut self, i: usize) {
        self.procs.remove(i);
    }

    pub fn highest_priority_ids(&self) -> Vec<usize> {
        let highest_p = self.highest_priority();
        let mut v: Vec<usize> = Vec::new();
        
        for i in 0..(self.procs.len()) {
            if self.procs[i].get_priority() == highest_p {
                v.push(i);
            }
        }
        v
    } 

    fn highest_priority(&self) -> u32 {
        if self.procs.len() == 0 {
            return std::u32::MAX;
        }
        let mut n: u32 = 0;
        for i in &self.procs {
            n = cmp::max(n, i.get_priority());
        }
        n
    }
}