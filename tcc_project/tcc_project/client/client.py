import requests
from file_transfers import FileTransfers
from xml_handler import*
from models import Course, Class, Video
from includes import *

class Client():
    def __init__(self, username, password, course, my_class, lecture, audio, video):
        self.client = ' '
        self.logged = False
        self.course = course
        self.my_class = my_class
        self.lecture = lecture
        self.audio = audio
        self.video = video
        self.video_path = self.get_video_path(course, my_class)

        if not self.logged:
            self.client, self.logged = self.login(username, password)
            if self.logged:
                self.username = username
                self.file_transfers = FileTransfers(self.client)                
               # self.teacher_fname = self.file_transfers.teacher_file_download(self.client, self.username)
                self.videos_remote_fname = self.file_transfers.video_file_download(self.client, course, my_class, self.username)
                #.file_transfers.file_upload(video_path, course, my_class, 'hehehehe', '10102010')
    
    def login(self, username, password):
        client = requests.session()
        #client.config['keep_alive'] = False
        # Retrieve the CSRF token first
        try:
            client.get(BASE_URL + LOGIN_URL)
        except:
            print('Connection Refused')
            return client, False
        if  client.cookies:
          csrftoken = client.cookies['csrftoken']
        else:
          csrftoken = 0 
        #login_data =  {'username':username, 'password':password, 'csrfmiddlewaretoken': csrftoken, 'next':'/'}
        #r = client.post( BASE_URL + LOGIN_URL, data=login_data, headers=dict(Referer= BASE_URL + LOGIN_URL))
        #if r.url == BASE_URL:
        success = True
        #else:
        #    success = False
        return client, success

    def video_sync(self):
        remote_xml_name = MEDIA_ROOT +  'videos/' + self.video_path + self.video_path.split('/')[-2] + '_remote.xml'
        local_xml_name = MEDIA_ROOT +  'videos/' + self.video_path + self.video_path.split('/')[-2] +'.xml'
        remote_missing_videos, remote_missing_audios = compare_files(remote_xml_name, local_xml_name, self.course, self.my_class)
        if isinstance(remote_missing_videos, int):
            if remote_missing_videos == -2:
                print('local xml file file not found')
            elif remote_missing_videos == -1:
                print('downloaded xml file file not found')
            else:
                print('unknown error')
            return 0
        else: 
            print('files compared: videos missing', len(remote_missing_videos), 'audios missing', len(remote_missing_audios))
            for video in remote_missing_videos:
                success = self.file_transfers.file_upload(video, 'hehehehe', "video")
                if(success == 1):
                    print('video uploaded')
                    self.erase_file(video.file)
                    remove_video_from_xml(video)
            for audio in remote_missing_audios:
                success = self.file_transfers.file_upload(audio, 'hehehehe', "audio")
                if(success == 1):
                    print('audio uploaded')
                    self.erase_file(audio.audio_file)
                    remove_audio_from_xml(audio)

    def get_video_path(self, course, my_class):
        path = course.code +'/'+ my_class.name + '/' + str(my_class.year) + '_' + str(my_class.semester) + '/'
        return path

    def erase_file(self, file_obj):
        os.remove(file_obj.name)

#login_data = dict(username=EMAIL, password=PASSWORD, csrfmiddlewaretoken=csrftoken, next='/')
#r = client.post(URL, data=login_data, headers=dict(Referer=URL))