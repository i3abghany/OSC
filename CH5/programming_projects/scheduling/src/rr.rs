use crate::proc::*;
use std::{borrow::BorrowMut, cmp};

pub struct RRScheduler {
    procs: Vec<Proc>,
}
const TIME_QUANTUM: u32 = 10;

impl RRScheduler {
    
    pub fn new() -> RRScheduler {
        RRScheduler {
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

    fn schedule(&mut self, n: usize) -> bool {
        if (self.procs[n]).get_rem() > TIME_QUANTUM {
            (self.procs[n]).schedule_for(TIME_QUANTUM);
            println!("Scheduled {} for {}", (self.procs[n]).get_name(), TIME_QUANTUM);
            false
        } else {
            let schedule_time: u32 = cmp::min(TIME_QUANTUM, (self.procs[n]).get_rem());
            (self.procs[n]).schedule_for(schedule_time);
            println!("Scheduled {} for {}", (self.procs[n]).get_name(), schedule_time);
            self.remove(n);
            true
        }
    }

    fn remove(&mut self, i: usize) {
        self.procs.remove(i);
    }

    pub fn operate(&mut self) {
        let mut n: usize = 0;
        while self.procs.len() > 0 {
            let b: bool = self.schedule(n);
            if !b {
                n = n + 1;
            }
            if n == (self.procs.len()) {
                n = 0;
            }
        }
    }
}