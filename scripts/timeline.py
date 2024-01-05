class Timeline:
    def __init__(self) -> None:
        self.timeline_frames = []
        self.next_empty_frame_id = 0

    def add_frame_data(self, frame_data):
        for frame in frame_data:
            timeline_frame = self.get_timeline_frame_data_for_start(frame.frame_start)
            timeline_frame.add_function
    
    def get_timeline_frame_data_for_start(self, start):
        for frame in self.timeline_frames:
            if(frame.start == start):
                return frame
        #no frame found, add new
        self.timeline_frames[self.next_empty_frame_id] = TimelineFrame(start)
        self.next_empty_frame_id = self.next_empty_frame_id+1
        return self.timeline_frames[self.next_empty_frame_id-1]

class TimelineFrame:
    def __init__(self, start) -> None:
        self.start = start
        self.functions = []

    def add_function(self, function):
        self.functions.append(function)