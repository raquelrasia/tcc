

class Course():
    def __init__(self, course_code):
        self.code = course_code

class Class():
    def __init__(self, class_name, class_year, class_semester):
        self.name = class_name
        self.year = class_year
        self.semester = class_semester

class Video():
    def __init__(self, course, my_class, video_date):
        self.date = video_date
        self.path = self._get_video_path(course, my_class) 
        self.path_name = path + date

    def _get_video_path(self, course, my_class):
        path =  course.code +'/'+ my_class.name + '/' + str(my_class.year) + '_' + my_class.semester + '/'
        return path
