import requests
from file_transfers import FileTransfers
from models import Course, Class, Video
BASE_URL = "http://127.0.0.1:8000/"
LOGIN_URL = "login/"
video_path = "teste2.ogv"
video_path2 = "amoieco.mp4"

class AccessServer():
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
                #self.file_transfers.file_upload(video_path)
                course = Course('EL68B')
                my_class = Class('S11', 2016, 1)
                self.teacher_fname = self.file_transfers.teacher_file_download(self.client, self.username)
                self.videos_remote_fname = self.file_transfers.video_file_download(self.client, course, my_class, self.username)
                self.video_sync(course, my_class)

                self.file_transfers.file_upload(video_path, course, my_class, 'hehehehe', '10102010')

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
        self.videos_fname = self.videos_remote_filename.split('_remote')
        print(self.videos_fname)
        # chama comparacao de arquivos xml
        self.file_transfers.compare_files()
        video_path = course + '/' + my_class + '/' + +str(year) + '_' + str(semester) 
        local_name = self.file_transfers.file_download(video_path)


    
if __name__ =="__main__":
    username = 'luis'
    password = 'teste'
    client_connection = AccessServer(username, password)




#login_data = dict(username=EMAIL, password=PASSWORD, csrfmiddlewaretoken=csrftoken, next='/')
#r = client.post(URL, data=login_data, headers=dict(Referer=URL))