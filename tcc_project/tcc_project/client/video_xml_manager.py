import sys
from xml_handler_client import *
from models import *
from client import Client


if len(sys.argv) != 8:
  print(len(sys.argv))
  sys.exit('not enough args')

course_code = str(sys.argv[1])
class_name = str(sys.argv[2])
year = str(sys.argv[3])
semester = str(sys.argv[4])
lecture_date = str(sys.argv[5])
video_path = str(sys.argv[6])
audio_path = str(sys.argv[7])

course = Course(course_code)
my_class = Class(class_name, year, semester, course)
lecture = Lecture(lecture_date, my_class)
video = Video({'name' : video_path}, lecture)
audio = Audio({'name' : audio_path}, lecture)
fname = create_xml_video_file(course, my_class)
add_lecture(lecture, fname)
add_video(video, fname )
add_audio(audio, fname )
print('file written')
print('connecting')
client_connection = Client('luis', ' ', course, my_class, lecture, video, audio)

sys.exit('connection ended')