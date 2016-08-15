import requests
import os
BASE_URL = "http://127.0.0.1:8000/"
VIDEO_UPLOAD_URL = "api/video_upload/"

class video_upload():
	def __init__(self, client):
		self.client = client


	def file_upload(self, video_path):
		csrftoken = self.client.cookies['csrftoken']
		print csrftoken
		payload = {'code': 'EL68B', 'tags': 'teste', 'csrfmiddlewaretoken': csrftoken}
		files = {
		     'file': (os.path.basename(video_path), open(video_path, 'rb'), 'application/octet-stream')
		}

		r = requests.post(BASE_URL + VIDEO_UPLOAD_URL, data=payload, files=files, headers=dict(Referer= BASE_URL + VIDEO_UPLOAD_URL))
		print(r.json())