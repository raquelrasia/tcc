import sys
from xml_handler import *
from models import *

fname = 'teste.txt';
f = open(fname, 'w')
f.close()

if len(sys.argv) != 6:
  print(len(sys.argv))
  sys.exit('not enough args')

fname = str(sys.argv[1])
course_code = str(sys.argv[1])
class_name = str(sys.argv[2])
lecture_date = str(sys.argv[3])
video_path = str(sys.argv[4])
audio_path = str(sys.argv[5])
if os.path.exists(fname) and os.path.getsize(fname) != 0:
	add_lecture(lecture_date, course_code, class_name, fname)
	add_video(video_path, lecture_date,  course_code,  class_name, fname )
	add_audio(audio_path, lecture_date,  course_code,  class_name, fname )
	sys.exit('file written')
else:
	sys.exit('cant find file')
print "My command line args are " + ca_one + " and " + ca_two