

class Course():
    def __init__(self, course_code):
        self.code = course_code

class Class():
    def __init__(self, class_name, class_year, class_semester):
        self.name = class_name
        self.year = class_year
        self.semester = class_semester

class Video():
    def __init__(self, course, my_class, video_date, video_name_path):
        self.date = video_date
        if(video_name_path.has_key('total_path')):
             self.path = video_name_path['total_path']
        elif video_name_path.has_key('name'):
             self.path = self._get_video_path(course, my_class) + video_name_path['name']            

    def _get_video_path(self, course, my_class):
        path =  course.code +'/'+ my_class.name + '/' + str(my_class.year) + '_' + str(my_class.semester) + '/'
        return path
