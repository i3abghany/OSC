#![allow(dead_code)]

use crate::proc::*;

pub struct FCFSScheduler {
    procs: Vec<Proc>,
}

impl FCFSScheduler {
    pub fn new() -> FCFSScheduler {
        FCFSScheduler {
            procs: Vec::new()
        }
    }

    pub fn add(&mut self, t: Proc) {
        self.procs.push(t);
    }

    pub fn get(&self, i: usize) -> &Proc {
        &self.procs[i]
    }

    pub fn get_next(&self) -> usize {
        return 0; // always the first Proc.
    }

    pub fn schedule(&mut self) {
        let next_idx: usize = self.get_next();
        let p = self.get(next_idx);
        println!("Scheduled {} for {}", p.get_name(), p.get_burst());
        self.remove(next_idx);
    }

    pub fn remove(&mut self, i: usize) {
        self.procs.remove(i);
    }

    pub fn operate(& mut self) {
        while self.procs.len() > 0 {
            self.schedule();
        }
    }
}
