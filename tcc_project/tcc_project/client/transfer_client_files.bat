@echo off
set _dir_macro=C:\Users\Raquel\Documents\Raquel\TCC\tcc_project\tcc_project\client\
set _dir_xml=C:\Users\Raquel\Documents\Raquel\TCC\tcc_project\tcc_project\lecture_recorder\
pscp -pw raspberry %_dir_xml%xml_handler.py %_dir_macro%video_xml_manager.py %_dir_macro%models.py %_dir_macro%client.py %_dir_macro%file_transfers.py %_dir_macro%includes.py %_dir_macro%files_sync.py  pi@169.254.96.87:/home/pi/client/
pscp -pw raspberry %_dir_macro%call_sync.py  pi@169.254.96.87:/usr/local/bin/
pause