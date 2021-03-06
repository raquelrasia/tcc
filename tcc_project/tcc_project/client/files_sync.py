import os
from models import *
from xml_handler import *
from file_transfers import *
from includes import *
#open xml files an run sync
from os import listdir
from os.path import isfile, join

accepted_video_formats = ['mp4', 'mp3' ,'mkv', 'ogv', 'webm']
accepted_audio_formats = ['mp3']

class FilesComparsion():
    def __init__(self, path):
        self.path = path
        self.course = ''
        self.my_class = ''
        self.video_path = ''

    def get_course_from_path(self):
        course_code = self.path.split('/')[1]
        self.course = Course(course_code = course_code)
        print('course code', course_code)

    def get_class_from_path(self):
        my_class_name = self.path.split('/')[2]
        year, semester = self.get_year_semester_from_path()
        self.my_class = Class(class_name = my_class_name, class_year = year, class_semester = semester,
                              course = self.course)
        print('class', my_class_name, year, semester)

    def get_year_semester_from_path(self):
        year_semester = self.path.split('/')[3]
        year, semester = year_semester.split('_')
        return year, semester

    def get_video_path(self):
        path = self.course.code +'/'+ self.my_class.name + '/' + str(self.my_class.year) + '_' + str(self.my_class.semester) + '/'
        self.video_path = path
        print('xml path', self.video_path)

    def  get_lecture_date_from_name(file_name):
        return file_name.split('.')[0]

if __name__ == "__main__":
    #for all dirs in videos
    file = open('/home/pi/kkk', 'w')
    file.write('funfou'):
    file.close()
    mydir = '/home/pi/videos'
    for path in [x[0] for x in os.walk(mydir)]:
        num_path_folders = len(path.split('/'))
        if (num_path_folders) == 7:
            path = path.split('/home/pi/')[-1]
            print(path)
            files_comparsion = FilesComparsion(path)
            files_comparsion.get_course_from_path();
            files_comparsion.get_class_from_path();
            files_comparsion.get_video_path();
            inner_dir_path = '/home/pi/'+ path + '/'
            files = [f for f in listdir(inner_dir_path) if isfile(join(inner_dir_path, f))]
            for file in files:
                is_video = False
                is_audio = False
                name, extension = os.path.splitext(file)
                extension = extension.split('.')[1]
                print(name, extension)
                if extension in accepted_video_formats:
                    is_video = True
                    video_path = inner_dir_path + file
                    audio_path = ''
                if extension in accepted_audio_formats:
                    is_audio = True
                    audio_path = inner_dir_path + file
                    video_path = ''
                if is_video or is_audio:
                    print(audio_path, video_path)
                    lecture_date = name.replace('_', '-')
                    #add all missing files to xml  
                    now = datetime.datetime.now()
                    test = now.month/3 + now.day
                    usr = 'unsyncd' + str(test)
                    course = files_comparsion.course
                    my_class = files_comparsion.my_class
                    os.system("video_xml_manager.py" + ourse.course_code + my_class.class_name, 
                              lecture_date, video_path, audio_path, usr)

