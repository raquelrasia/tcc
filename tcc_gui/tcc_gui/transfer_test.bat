ssh pi@169.254.96.87 "mkdir -p /home/pi/Videos/IF66D/S11/2016_2/"
pscp -pw raspberry C:/Users/luigu/Videos/LectureRecordings/IF66D/S11/2016_2/15_09_2016.mkv C:/Users/luigu/Videos/LectureRecordings/IF66D/S11/2016_2/15_09_2016.mp3 pi@169.254.96.87:Videos/IF66D/S11/2016_2/
