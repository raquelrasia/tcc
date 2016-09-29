import sys
from xml_handler import *
from models import *
from client import Client


if len(sys.argv) != 9:
  print(len(sys.argv))
  sys.exit('not enough args')

course_code = str(sys.argv[1])
class_name = str(sys.argv[2])
year = str(sys.argv[3])
semester = str(sys.argv[4])
lecture_date = str(sys.argv[5])
video_path = str(sys.argv[6])
audio_path = str(sys.argv[7])
username = str(sys.argv[8])

course = Course(course_code)
my_class = Class(class_name, year, semester, course)
lecture = Lecture(lecture_date, my_class)
video = Video({'name' : video_path}, lecture)
audio = Audio({'name' : audio_path}, lecture)
fname = create_xml_video_file(course, my_class)
new_lecture = add_lecture(lecture, fname)
if(video_path != ''):
	new_video = add_video(video, fname)
if(audio_path != ''):
	new_audio = add_audio(audio, fname)
#if(new_video or new_audio):
print('connecting')
client_connection = Client(username, ' ', course, my_class, lecture, video, audio)
add_state = client_connection.video_sync()
print('files_uploaded')
#else:
#	print('no files to upload')
sys.exit('connection ended')