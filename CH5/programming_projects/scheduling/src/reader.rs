use std::fs::File;
use std::io::Read;
use crate::proc::*;

pub struct ProcReader {
    file_name: String,
    file_content: String,
    procs_raw: Vec<String>,
    file: Option<File>,
}

impl ProcReader {
    fn open(&mut self, fname: String) {
        self.file_name = fname.clone(); 
        self.file = Some(File::open(fname)
            .expect("Could not open the trace file."));
        
        self.read_content();
    }

    fn read_content(&mut self) {
        self.file.as_ref().unwrap().read_to_string(&mut self.file_content)
            .expect("Could not read data from the trace file.");

        let strs: Vec<&str> = self.file_content.split('\n').collect();
        for &item in &strs {
            self.procs_raw.push(String::from(item));
        }

        // println!("{:?}", self.read_proc())
    }

    pub fn new(fname: String) -> ProcReader {
        let mut res = ProcReader {
            file_name: String::new(),
            file_content: String::new(),    
            file: None,
            procs_raw: Vec::new(),
        };
        res.open(fname);

        return res;
    }

    pub fn print_content(&self) {
        println!("{}", self.file_content);
    }

    pub fn read_proc(&mut self) -> Option<Proc> {
        if self.procs_raw.len() == 0 {
            return None
        } else {
            let els: Vec<&str> = self.procs_raw[0].split(',').collect();
            let p = Proc::new(
                String::from(els[0]),
                els[1].trim().parse().unwrap(),
                els[2].trim().parse().unwrap(),
                els[1].trim().parse().unwrap(),
            );
            self.procs_raw.remove(0);
            return Some(p);
        }
    }
}