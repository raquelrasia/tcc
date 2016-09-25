import requests
import os
import datetime
from models import Course, Class, Video
from xml_handler_client import prettify_all
from includes import *
import platform

class FileTransfers():
    def __init__(self, client):
        self.client = client

    def file_upload(self, file, tag_list, video_audio):
        lecture = file.lecture
        my_class = lecture.my_class
        course = my_class.course
        csrftoken = self.client.cookies['csrftoken']
        payload = {'course_code': course.code, 'class_name': my_class.name, 'class_year': my_class.year, 
                   'class_semester': my_class.semester, 'tags': 'teste', 'date': datetime.datetime.strptime(lecture.date_name, "%d-%m-%Y"), 
                   'csrfmiddlewaretoken': csrftoken}
        if video_audio == "video":
            file_name  = file.file.name
        elif video_audio == "audio":
            file_name =file.audio_file.name 
        files = {
             'file': (os.path.basename(MEDIA_ROOT + file_name), open(MEDIA_ROOT + file_name, 'rb'), 'application/octet-stream')
        }

        if video_audio == "video":
            url = VIDEO_UPLOAD_URL
        elif video_audio == "audio":
            url = AUDIO_UPLOAD_URL

        r = requests.post(BASE_URL + url, data=payload, files=files, headers=dict(Referer= BASE_URL + url))
        print(r.json())

    def teacher_file_download(self, client, username):
        #csrftoken = self.client.cookies['csrftoken']
        csrftoken = 'kkk'
        payload = {'teacher_username': username, 'csrfmiddlewaretoken': csrftoken}
        local_filename = MEDIA_ROOT + 'config_' + username + '.xml'
        # NOTE the stream=True parameter
        r = requests.get(BASE_URL + TEACHER_INFO_DOWNLOAD_URL, stream=True, params=payload)
        if not os.path.exists(os.path.dirname(local_filename)):
            os.makedirs(os.path.dirname(local_filename))
            print(local_filename)
            
        with open(local_filename, 'wb') as f:
            for chunk in r.iter_content(chunk_size=1024): 
                if chunk: # filter out keep-alive new chunks
                    f.write(chunk)
        print('teacher file downloaded')
        return local_filename


    def video_file_download(self, client, course, my_class, username):
        #csrftoken = self.client.cookies['csrftoken']
        csrftoken = 'kkk'
        payload = {'course_code': course.code, 'class_name': my_class.name, 
                   'class_semester' : my_class.semester, 'class_year' : my_class.year, 
                   'username' : username , 'csrfmiddlewaretoken': csrftoken}

        remote_filename = MEDIA_ROOT +'/videos/' + course.code + '/' + my_class.name + '/'+ str(my_class.year) + '_'+ str(my_class.semester) + '/'
        names_split = remote_filename.split('/')
        name = names_split[-2] + names_split[-1]
        remote_filename = remote_filename + name +'_remote.xml'
        # NOTE the stream=True parameter
        r = requests.get(BASE_URL + VIDEO_INFO_DOWNLOAD_URL, stream=True, params=payload)
        if not os.path.exists(os.path.dirname(remote_filename)):
            os.makedirs(os.path.dirname(remote_filename)) 
        with open(remote_filename+'_ugly', 'wb') as f:
            for chunk in r.iter_content(chunk_size=1024): 
                if chunk: # filter out keep-alive new chunks
                    f.write(chunk)

        prettyfied = prettify_all(remote_filename +'_ugly')
        with open(remote_filename, 'w') as f:
            f.write(prettyfied)
        
        print('video file downloaded')
        return remote_filename

            