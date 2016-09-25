import requests
from file_transfers import FileTransfers
from xml_handler import*
from models import Course, Class, Video
BASE_URL = "http://127.0.0.1:8000/"
LOGIN_URL = "login/"
video_path = "teste2.ogv"
video_path2 = "amoieco.mp4"
MEDIA_ROOT = 'files/'

class Client():
    def __init__(self, username, password):
        self.client = ' '
        self.logged = False
        if not self.logged:
            self.client, self.logged = self.login(username, password)
            self.logged = True
            if self.logged:
                self.username = username
                print('connection successfull')
                self.file_transfers = FileTransfers(self.client)
                course = Course('EL64A')
                my_class = Class('S11', 2015, 1)
                self.video_path = self.get_video_path(course, my_class)                
                self.teacher_fname = self.file_transfers.teacher_file_download(self.client, self.username)
                self.videos_remote_fname = self.file_transfers.video_file_download(self.client, course, my_class, self.username)
                self.video_sync(course, my_class)
                #.file_transfers.file_upload(video_path, course, my_class, 'hehehehe', '10102010')

    def login(self, username, password):
        client = requests.session()
        # Retrieve the CSRF token first
        client.get(BASE_URL + LOGIN_URL)
        if  client.cookies:
          csrftoken = client.cookies['csrftoken']
        else:
          csrftoken = 0 
        login_data =  {'username':username, 'password':password, 'csrfmiddlewaretoken': csrftoken, 'next':'/'}
        r = client.post( BASE_URL + LOGIN_URL, data=login_data, headers=dict(Referer= BASE_URL + LOGIN_URL))
        if r.url == BASE_URL:
            success = True
        else:
            success = False
        return client, success

    def video_sync(self, course, my_class):
        remote_xml_name =  MEDIA_ROOT + 'videos/' + self.video_path + self.video_path.split('/')[-2] + '_remote.xml'
        local_xml_name =  MEDIA_ROOT + 'videos/' + self.video_path + self.video_path.split('/')[-2] +'.xml'
        remote_missing_videos, remote_missing_audio = compare_files(remote_xml_name, local_xml_name, course, my_class)

        if isinstance(remote_missing_videos, int):
            if remote_missing_videos == -2:
                print('local xml file file not found')
            elif remote_missing_videos == -1:
                print('downloaded xml file file not found')
            else:
                print('unknown error')
        else: 
            for video in remote_missing_videos:
                self.file_transfers.file_upload(video, course, my_class, 'hehehehe', "video")
            
            for audio in remote_missing_videos:
                self.file_transfers.file_upload(audio, course, my_class, 'hehehehe', "audio")

    def _add_video_to_xml(course, my_class, lecture, video):
        fname = create_xml_video_file(course, my_class)
        add_lecture(lecture, fname)
        add_video(video, fname)

    def _add_audio_to_xml(course, my_class, lecture, audio):
        fname = create_xml_video_file(course, my_class)
        add_lecture(lecture, fname)
        add_audio(audio, fname)

    def _remove_video_from_xml(course, my_class, video):
        fname = get_videos_xml_name(course, my_class)
        remove_video(video, fname)

    def _remove_audio_from_xml(course, my_class, audio):
        fname = get_videos_xml_name(course, my_class)
        remove_audio(audio, fname)
            

    def get_video_path(self, course, my_class):
        path = course.code +'/'+ my_class.name + '/' + str(my_class.year) + '_' + str(my_class.semester) + '/'
        return path

if __name__ =="__main__":

    username = 'luis'
    password = 'teste'
    client_connection = Client(username, password)




#login_data = dict(username=EMAIL, password=PASSWORD, csrfmiddlewaretoken=csrftoken, next='/')
#r = client.post(URL, data=login_data, headers=dict(Referer=URL))