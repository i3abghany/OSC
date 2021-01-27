use crate::proc::*;
use std::cmp;
pub struct PriorityRRScheduler {
    procs: Vec<Proc>,
    clock: u32,
    proc_num: usize,
    hs: std::collections::HashSet<usize>,
    int_vec: Vec<usize>,
}

const TIME_QUANTUM: u32 = 10;

impl PriorityRRScheduler {
    
    pub fn new() -> PriorityRRScheduler {


        PriorityRRScheduler {
            procs: Vec::new(),
            clock: 0,
            proc_num: 0,
            hs: std::collections::HashSet::new(),
            int_vec: Vec::new(),            
        }
    }

    pub fn add(&mut self, t: Proc) {
        self.procs.push(t);
    }

    fn get(&self, i: usize) -> &Proc {
        &self.procs[i]
    }

    fn schedule_equal_priority(&mut self, v: &mut Vec<usize>) {
        loop {
            for i in 0..v.len() {
                let i = v[i];
                if self.hs.contains(&i) {
                    continue;
                }
                if self.procs[i].get_rem() > TIME_QUANTUM {
                    self.schedule_proc_intermediate(i);
                } else {
                    self.hs.insert(i);
                    self.finalize_proc(i);
                }
            }

            if v.iter().all(|x| self.hs.contains(x)) {
                break;
            }
        }

        // self.remove_bulk(v);
    }

    fn schedule_proc_intermediate(&mut self, i: usize) {
        if self.procs[i].get_burst() == self.procs[i].get_rem() {
            self.procs[i].respond(self.clock);
        }
        self.schedule_proc_for(i, TIME_QUANTUM);
        println!();
    }

    fn schedule_proc_for(&mut self, i: usize, n: u32) {
        print!("Scheduled {} for {}", self.procs[i].get_name(), n);
        self.procs[i].schedule_for(n);
        self.clock += n;
        self.all_wait_but(i, n)
    }

    fn finalize_proc(&mut self, i: usize) {
        let schedule_time: u32 = cmp::min(TIME_QUANTUM, self.procs[i].get_rem());
        self.schedule_proc_for(i, schedule_time);
        print!(" (finished)\n");
        self.procs[i].turnaround(self.clock);
    }

    fn all_wait_but(&mut self, idx: usize, n: u32) {
        for i in 0..self.procs.len() {
            if idx == i || self.hs.contains(&i) {
                continue;
            }
            self.procs[i].wait_for(n);
        }
    }

    pub fn operate(&mut self) {
        self.proc_num = self.procs.len();

        for i in 0..self.proc_num {
            self.int_vec.push(i);
        }

        let mut highest_p = self.highest_priority();
        while highest_p != std::u32::MAX {
            let mut v = self.highest_priority_ids();
            self.schedule_equal_priority(&mut v);
            highest_p = self.highest_priority();
        }
        self.stats();
    }

    fn remove(&mut self, i: usize) {
        self.procs.remove(i);
    }

    pub fn highest_priority_ids(&self) -> Vec<usize> {
        let highest_p = self.highest_priority();
        let mut v: Vec<usize> = Vec::new();
        
        for i in 0..(self.procs.len()) {
            if self.procs[i].get_priority() == highest_p && !self.hs.contains(&i) {
                v.push(i);
            }
        }
        v
    } 

    fn all_finished(&self) -> bool {
        self.int_vec.iter().all(|x| self.hs.contains(x))
    }

    fn highest_priority(&self) -> u32 {
        if  self.all_finished() {
            return std::u32::MAX;
        }

        let mut n: u32 = 0;
        for i in 0..self.procs.len() {
            if self.hs.contains(&i) {
                continue;
            }
            n = cmp::max(n, self.procs[i].get_priority());
        }
        n
    }

    fn stats(&self) {
        println!("Average turnaround time: {}", (self.get_total_turnaround_time() as f32) / (self.proc_num as f32));
        println!("Average wait time: {}", (self.get_total_wait_time() as f32) / (self.proc_num as f32));
        println!("Average response time: {}", (self.get_total_response_time() as f32) / (self.proc_num as f32));
    }

    fn get_total_wait_time(&self) -> u32 {
        let mut n: u32 = 0;

        for i in 0..self.procs.len() {
            n += self.procs[i].get_wait_time();
        }

        n
    }

    fn get_total_turnaround_time(&self) -> u32 {
        let mut n: u32 = 0;

        for i in 0..self.procs.len() {
            n += self.procs[i].get_turnaround_time();
        }

        n
    }

    fn get_total_response_time(&self) -> u32 {
        let mut n: u32 = 0;

        for i in 0..self.procs.len() {
            n += self.procs[i].get_response_time();
        }

        n
    }
}