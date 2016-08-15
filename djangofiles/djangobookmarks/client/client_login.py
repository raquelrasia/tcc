import requests
from video_upload import video_upload
BASE_URL = "http://127.0.0.1:8000/"
LOGIN_URL = "login/"

video_path = "teste2.ogv"
video_path2 = "amoieco.mp4"
class access_server():
    def __init__(self, username, password):
        self.client = ' '
        self.logged = False
        if not self.logged:
            self.client, self.logged = self.login(username, password)
            if self.logged:
                print('connection successfull')
                v_upload = video_upload(self.client)
                v_upload.file_upload(video_path)

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
    
if __name__ =="__main__":
    client_connection = access_server('teste', 'teste')




#login_data = dict(username=EMAIL, password=PASSWORD, csrfmiddlewaretoken=csrftoken, next='/')
#r = client.post(URL, data=login_data, headers=dict(Referer=URL))