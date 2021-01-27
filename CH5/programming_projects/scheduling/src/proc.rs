#[derive(Debug, Clone)]
pub struct Proc {
    name: String,
    cpu_burst: u32, 
    priority: u32,
    time_rem: u32,
}

impl Proc {
    pub fn new(name: String,
        cpu_burst: u32,
        priority: u32,
        time_rem: u32) -> Proc {
        Proc {
            name, 
            cpu_burst,
            priority,
            time_rem,
        }
    }

    pub fn get_name(&self) -> &str {
        &self.name
    }

    pub fn get_burst(&mut self) -> u32 {
        self.cpu_burst
    }

    pub fn get_priority(&self) -> u32 {
        self.priority
    }

    pub fn get_rem(&mut self) -> u32 {
        self.time_rem
    }

    pub fn schedule_for(&mut self, n: u32) {
        self.time_rem = self.time_rem - n;
    }
}
