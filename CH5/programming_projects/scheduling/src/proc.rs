#[derive(Debug, Clone)]
pub struct Proc {
    name: String,
    cpu_burst: u32, 
    priority: u32,
    time_rem: u32,
    turnaround_time: u32,
    wait_time: u32,
    response_time: u32
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
            turnaround_time: 0,
            wait_time: 0,
            response_time: 0,
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

    pub fn wait_for(&mut self, n: u32) {
        self.wait_time += n;
    }

    pub fn respond(&mut self, n: u32) {
        self.response_time = n;
    }

    pub fn turnaround(&mut self, n: u32) {
        self.turnaround_time = n;
    }

    pub fn get_wait_time(&self) -> u32 {
        self.wait_time
    }

    pub fn get_turnaround_time(&self) -> u32 {
        self.turnaround_time
    }
    
    pub fn get_response_time(&self) -> u32 {
        self.response_time
    }

}
