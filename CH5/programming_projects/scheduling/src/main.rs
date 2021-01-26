#![allow(dead_code)]


mod proc;
mod reader;

mod sjf;
use sjf::*;

use reader::*;

fn main() {
    let mut sjf_sched = SJFScheduler::new();
    let mut rd = ProcReader::new(String::from("./traces/fcfs.txt"));

    loop {
        let prc = rd.read_proc();
        match prc {
            Some(p) => sjf_sched.add(p),
            None => break,
        }
    }

   sjf_sched.operate(); 
}
