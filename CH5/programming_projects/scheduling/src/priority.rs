#![allow(dead_code)]

use crate::proc::*;

pub struct PriorityScheduler {
    procs: Vec<Proc>,
}

impl PriorityScheduler {
    pub fn new() -> PriorityScheduler {
        PriorityScheduler {
            procs: Vec::new()
        }
    }

    pub fn add(&mut self, t: Proc) {
        self.procs.push(t);
    }

    fn get(&self, i: usize) -> &Proc {
        &self.procs[i]
    }

    fn get_next(&self) -> usize {
        let mut p = self.get(0);
        let mut idx: usize = 0;
        let sz = self.procs.len();

        for i in 1..sz {
            let curr_proc = self.get(i);
            if p.get_priority() < curr_proc.get_priority() {
                p = curr_proc;
                idx = i;
            }
        }
        return idx;
    }

    fn schedule(&mut self) {
        let next_idx: usize = self.get_next();
        let p = self.get(next_idx);
        println!("Scheduled {} for {}", p.get_name(), p.get_burst());
        self.remove(next_idx);
    }

    fn remove(&mut self, i: usize) {
        self.procs.remove(i);
    }

    pub fn operate(& mut self) {
        while self.procs.len() > 0 {
            self.schedule();
        }
    }
}
