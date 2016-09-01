import requests
import os
import datetime
from models import Course, Class, Video
BASE_URL = "http://127.0.0.1:8000/"
VIDEO_UPLOAD_URL = "api/video_upload/"
TEACHER_INFO_DOWNLOAD_URL = 'api/teacher_xml_download/'
VIDEO_INFO_DOWNLOAD_URL = 'api/video_xml_download/'
ROOT = 'files/'

class FileTransfers():
    def __init__(self, client):
        self.client = client

    def file_upload(self, video_path, course, my_class, tag_list, date):
        csrftoken = self.client.cookies['csrftoken']
        payload = {'course_code': course.code, 'class_name': my_class.name, 'class_year': my_class.year, 
                   'class_semester': my_class.semester, 'tags': 'teste', 'date': datetime.datetime.strptime(date, "%d%m%Y"), 
                   'csrfmiddlewaretoken': csrftoken}
        files = {
             'file': (os.path.basename(video_path), open(video_path, 'rb'), 'application/octet-stream')
        }

        r = requests.post(BASE_URL + VIDEO_UPLOAD_URL, data=payload, files=files, headers=dict(Referer= BASE_URL + VIDEO_UPLOAD_URL))
        print(r.json())

    def teacher_file_download(self, client, username):
        #csrftoken = self.client.cookies['csrftoken']
        csrftoken = 'kkk'
        payload = {'teacher_username': username, 'csrfmiddlewaretoken': csrftoken}
        local_filename = ROOT + 'config_' + username + '.xml'
        # NOTE the stream=True parameter
        r = requests.get(BASE_URL + TEACHER_INFO_DOWNLOAD_URL, stream=True, params=payload)
        if not os.path.exists(os.path.dirname(local_filename)):
            os.makedirs(os.path.dirname(local_filename))
            print(local_filename)
            
        with open(local_filename, 'wb') as f:
            for chunk in r.iter_content(chunk_size=1024): 
                if chunk: # filter out keep-alive new chunks
                    f.write(chunk)
        return local_filename


    def video_file_download(self, client, course, my_class, username):
        #csrftoken = self.client.cookies['csrftoken']
        csrftoken = 'kkk'
        payload = {'course_code': course.code, 'class_name': my_class.name, 
                   'class_semester' : my_class.semester, 'class_year' : my_class.year, 
                   'username' : username , 'csrfmiddlewaretoken': csrftoken}

        local_filename = ROOT +'/videos/' + course.code + '/' + my_class.name + '/'+ str(my_class.year) + '_'+ str(my_class.semester) + '/'
        names_split = local_filename.split('/')
        name = names_split[-2] + names_split[-1]
        local_filename = local_filename + name +'_remote.xml'
        # NOTE the stream=True parameter
        r = requests.get(BASE_URL + VIDEO_INFO_DOWNLOAD_URL, stream=True, params=payload)
        print(len(r.content))
        if not os.path.exists(os.path.dirname(local_filename)):
            os.makedirs(os.path.dirname(local_filename))        
        with open(local_filename, 'wb') as f:
            for chunk in r.iter_content(chunk_size=1024): 
                if chunk: # filter out keep-alive new chunks
                    f.write(chunk)
        return local_filename