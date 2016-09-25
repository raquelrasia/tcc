import datetime
import os
MEDIA_ROOT = 'videos'
class Course():
    def __init__(self, course_code):
        self.code = course_code

class Class():
    def __init__(self, class_name, class_year, class_semester, course):
        self.name = class_name
        self.year = class_year
        self.semester = class_semester
        self.course = course
class Lecture():
    def __init__(self, date_name, my_class):
        self.date = datetime.datetime.strptime(date_name, "%d-%m-%Y").date()
        self.date_name = date_name
        self.my_class = my_class

class File():
    def __init__(self, path):
       self.name = path
       if os.path.exists(path):
            self.size = os.path.getsize(path)
       else:
            self.size = 0
class Video():
    def __init__(self, video_name_path, lecture):
        my_class = lecture.my_class
        course = my_class.course
        if(video_name_path.has_key('total_path')):
             self.path = video_name_path['total_path']
        elif video_name_path.has_key('name'):
             self.path = self._get_video_path(course, my_class) + video_name_path['name']   
        self.file = File(self.path)
        self.name = os.path.basename(self.path)
        self.lecture = lecture
    def _get_video_path(self, course, my_class):
        path =  MEDIA_ROOT + '/'+course.code +'/'+ my_class.name + '/' + str(my_class.year) + '_' + str(my_class.semester) + '/'
        return path

class Audio():
    def __init__(self, audio_name_path, lecture):
        my_class = lecture.my_class
        course = my_class.course
        if(audio_name_path.has_key('total_path')):
             self.audio_path = audio_name_path['total_path']
        elif audio_name_path.has_key('name'):
             self.audio_path = self._get_audio_path(course, my_class) + audio_name_path['name']            
        self.audio_file = File(self.audio_path)
        self.audio_name = os.path.basename(self.audio_path)
        self.lecture = lecture
    def _get_audio_path(self, course, my_class):
        path = MEDIA_ROOT + '/'+ course.code +'/'+ my_class.name + '/' + str(my_class.year) + '_' + str(my_class.semester) + '/'
        return path
