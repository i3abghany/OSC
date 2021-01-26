#![allow(dead_code)]

mod fcfs;
mod proc;
mod reader;

use fcfs::*;
use reader::*;

fn main() {
    let mut fcfs_sched = FCFSScheduler::new();
    let mut rd = ProcReader::new(String::from("./traces/fcfs.txt"));

    loop {
        let prc = rd.read_proc();
        match prc {
            Some(p) => fcfs_sched.add(p),
            None => break,
        }
    }

   fcfs_sched.operate(); 
}
