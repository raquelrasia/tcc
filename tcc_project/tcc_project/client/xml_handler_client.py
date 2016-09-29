import xml.etree.cElementTree as ET
import os
import datetime
from xml.dom import minidom
from models import Video, Audio, Lecture, MEDIA_ROOT



def get_course_class_path(course, my_class):
    path = course.code +'/'+ my_class.name + '/' + str(my_class.year) + '_' + my_class.semester + '/'
    return path

def get_videos_xml_name(course, my_class):
    path = MEDIA_ROOT +'/'+ get_course_class_path(course, my_class)
    names_split = path.split('/')
    name = names_split[-2] + names_split[-1]
    fname =  path + name + '.xml'
    return fname

def create_xml_video_file(course, my_class): 
    fname =  get_videos_xml_name(course, my_class)
    if os.path.exists(fname) and os.path.getsize(fname) != 0:
        return fname
    else:
        if not os.path.exists(os.path.dirname(fname)):
            os.makedirs(os.path.dirname(fname))
        root = ET.Element('root')
    f = open(fname, 'w')
    course_et = ET.SubElement(root, 'course')
    course_et.set('code', course.code)
    class_et = ET.SubElement(course_et, 'class')
    class_et.set('name', my_class.name)
    f.write(ET.tostring(root))
    f.close()
    return fname

def add_video(video, fname):
    if find_video(video, fname)
        return 0
    lecture = video.lecture
    my_class = lecture.my_class
    course = my_class.course
    tree = ET.parse(fname)
    root = tree.getroot()
    exists = False
    for course_et in root.findall('course'):
        if course_et.get('code') == course.code:
            for class_et in course_et.findall('class'):
                if my_class.name == class_et.get('name'):
                    for lecture_et in class_et.findall('lecture'):
                        if lecture.date_name == lecture_et.get('date'):
                            exists = True
                            video_et = ET.SubElement(lecture_et, 'video')
                            video_et.set( 'name', video.name)
                            video_et.set( 'path', video.file.name)
                            size = ET.SubElement(video_et, 'size')
                            size.text = str(video.file.size)
    if exists:
        f = open(fname, 'w')
        ET.tostring(root)
        #prettify(video_et)
        #f.write(ET.tostring(root))
        f.write(ET.tostring(root))#ET.tostring(root))
        f.close()
        return 1
    else:
        return -1

def add_audio(audio, fname):
    if find_audio(audio, fname)
        return 0
    lecture = audio.lecture
    my_class = lecture.my_class
    course = my_class.course
    tree = ET.parse(fname)
    root = tree.getroot()
    exists = False
    for course_et in root.findall('course'):
        if course_et.get('code') == course.code:
            for class_et in course_et.findall('class'):
                if my_class.name == class_et.get('name'):
                    for lecture_et in class_et.findall('lecture'):
                        if lecture.date_name == lecture_et.get('date'):
                            exists = True
                            audio_et = ET.SubElement(lecture_et, 'audio')
                            audio_et.set( 'name', audio.audio_name)
                            audio_et.set( 'path', audio.audio_file.name)
                            size = ET.SubElement(audio_et, 'size')
                            size.text = str(audio.audio_file.size)
    if exists:
        f = open(fname, 'w')
        ET.tostring(root)
        #prettify(video_et)
        #f.write(ET.tostring(root))
        f.write(ET.tostring(root))#ET.tostring(root))
        f.close()
        return 1
    else:
        return -1
def remove_audio(audio, fname):
    lecture = audio.lecture
    my_class = lecture.my_class
    course = my_class.course
    if not os.path.exists(fname) or os.path.getsize(fname) == 0:
        return -1

    tree = ET.parse(fname)
    root = tree.getroot()
    exists = False
    for course_et in root.findall('course'):
        if course_et.get('code') == course.code:
            for class_et in course_et.findall('class'):
                if my_class.name == class_et.get('name'):
                    for lecture_et in class_et.findall('lecture'):
                        if lecture.date_name == lecture_et.get('date'):
                            for audio_et in lecture_et.findall('audio'):
                                if audio_et.get('name') == audio.audio_name and
                                   audio_et.get('path') == audio.audio_file.name:
                                    lecture_et.remove(audio_et)
                                    exists = True
    if exists:
        f = open(fname, 'w')
        ET.tostring(root)
        #prettify(video_et)
        #f.write(ET.tostring(root))
        f.write(ET.tostring(root))#ET.tostring(root))
        f.close()

def remove_video(video, fname):
    lecture = video.lecture
    my_class = lecture.my_class
    course = my_class.course
    if not os.path.exists(fname) or os.path.getsize(fname) == 0:
        return -1
        
    tree = ET.parse(fname)
    root = tree.getroot()
    exists = False
    for course_et in root.findall('course'):
        if course_et.get('code') == course.code:
            for class_et in course_et.findall('class'):
                if my_class.name == class_et.get('name'):
                    for lecture_et in class_et.findall('lecture'):
                        if lecture.date_name == lecture_et.get('date'):
                            for video_et in lecture_et.findall('video'):
                                if video_et.get('name') == video.name and
                                   video_et.get('path') == video.file.name:
                                    lecture_et.remove(video_et)
                                    exists = True
    if exists:
        f = open(fname, 'w')
        ET.tostring(root)
        #prettify(video_et)
        #f.write(ET.tostring(root))
        f.write(ET.tostring(root))#ET.tostring(root))
        f.close()

def find_audio(audio, fname):
    lecture = audio.lecture
    my_class = lecture.my_class
    course = my_class.course
    if not os.path.exists(fname) or os.path.getsize(fname) == 0:
        return -1

    tree = ET.parse(fname)
    root = tree.getroot()
    exists = False
    for course_et in root.findall('course'):
        if course_et.get('code') == course.code:
            for class_et in course_et.findall('class'):
                if my_class.name == class_et.get('name'):
                    for lecture_et in class_et.findall('lecture'):
                        if lecture.date_name == lecture_et.get('date'):
                            for audio_et in lecture_et.findall('audio'):
                                if audio_et.get('name') == audio.audio_name and
                                   audio_et.get('path') == audio.audio_file.name:
                                   exists = True
                                   break
    return exists

def find_video(video, fname):
    lecture = video.lecture
    my_class = lecture.my_class
    course = my_class.course
    if not os.path.exists(fname) or os.path.getsize(fname) == 0:
        return -1
        
    tree = ET.parse(fname)
    root = tree.getroot()
    exists = False
    for course_et in root.findall('course'):
        if course_et.get('code') == course.code:
            for class_et in course_et.findall('class'):
                if my_class.name == class_et.get('name'):
                    for lecture_et in class_et.findall('lecture'):
                        if lecture.date_name == lecture_et.get('date'):
                            for video_et in lecture_et.findall('video'):
                                if video_et.get('name') == video.name and
                                   video_et.get('path') == video.file.name:
                                    exists = True
                                    break
    return exists

def create_xml_teacher_file(user): 
    f_name = MEDIA_ROOT +'teachers/'+ user.username
    fname =  f_name + '.xml'
    if os.path.exists(fname) and os.path.getsize(fname) != 0:
        return fname
    else:
        if not os.path.exists(os.path.dirname(fname)):
            os.makedirs(os.path.dirname(fname))
    root = ET.Element('root')
    f = open(fname, 'w')
    teacher_et = ET.SubElement(root, 'teacher')
    teacher_et.set('username', user.username)
    f.write(ET.tostring(root))
    f.close()

            
def add_course_class(course, my_class, user):
    f_name = MEDIA_ROOT +'teachers/'+ user.username
    fname =  f_name + '.xml'
    tree = ET.parse(fname)
    root = tree.getroot()
    existing_class = False
    teacher_et = root.findall('teacher')[0]
    if teacher_et.get('username') == user.username:
        for course_et in teacher_et.findall('course'):
            code = course_et.get('code')
            if code == course.code:
                #nova class em curso ja cadastrado
                existing_class = True
                _add_class(course_et, my_class)
        if not existing_class:
              course_et =  ET.SubElement(teacher_et, 'course')
              course_et.set('code', str(course.code))
              course_et.set('name', str(course.name))
              _add_class(course_et, my_class)
        f = open(fname, 'w')
        ET.tostring(root)
        #prettify(video_et)
        #f.write(ET.tostring(root))
        f.write(ET.tostring(root))#ET.tostring(root))
        f.close()

def add_lecture(lecture, fname):
    my_class = lecture.my_class
    course = my_class.course
    tree = ET.parse(fname)
    root = tree.getroot()
    existing_class_course = False
    lecture_exists = False
    for course_et in root.findall('course'):
        if course_et.get('code') == course.code:
            for class_et in course_et.findall('class'):
                if my_class.name == class_et.get('name'):
                    for lecture_et in class_et.findall('lecture'):
                        if(lecture_et.get('date') == lecture.date_name):
                            lecture_exists = True
                            break
                    if not lecture_exists:
                        existing_class_course = True
                        lecture_et = ET.SubElement(class_et, 'lecture')
                        lecture_et.set('date', str(lecture.date.strftime('%d-%m-%Y')))
    if existing_class_course:
        f = open(fname, 'w')
        ET.tostring(root)
        #prettify(video_et)
        #f.write(ET.tostring(root))
        f.write(ET.tostring(root))#ET.tostring(root))
        f.close()



def _add_class( course_node, my_class):
    class_et =  ET.SubElement(course_node, 'class')
    class_et.set('name', my_class.name)
    class_et.set('year', str(my_class.year))
    class_et.set('semester', str(my_class.semester))
     

def compare_files( f_remote, f_local, course, my_class):
    print(os.path.abspath(f_remote))
    if not os.path.isfile(f_remote):
        return -1, -1
    if not os.path.isfile(f_local):
        return -2, -2
    missing_video_files = list()
    missing_audio_files = list()
    remote_video_list = list()
    local_video_list = list()
    remote_video_list, remote_audio_list = _return_video_audio_list(f_remote, course, my_class)
    local_video_list, local_audio_list = _return_video_audio_list(f_local, course, my_class)
    video_found = False
    audio_found = False
    for video in local_video_list:
        video_found = False
        for video_remote in remote_video_list:
            if video.file.name == video_remote.file.name:
                video_found = True
                break
        if not video_found and video not in missing_video_files:
            #file needs to be uploaded
            print ('missing video', video.file.name)
            missing_video_files.append(video)
    for audio in local_audio_list:
        audio_found = False
        for audio_remote in remote_audio_list:
            if audio.audio_file.name == audio_remote.audio_file.name:
                audio_found = True
                break
        if not audio_found and audio not in missing_audio_files:
            #file needs to be uploaded
            print ('missing audio', audio.audio_file.name)
            missing_audio_files.append(audio)
    return missing_video_files, missing_audio_files 


def _return_video_audio_list( fname, course, my_class):
    tree = ET.parse(fname)
    root = tree.getroot()
    correct_class = False
    correct_course = False
    video_list = list()
    audio_list = list()
    for course_et in root.findall('course'):
        code = course_et.get('code')
        if code == course.code:
            correct_course = True
            for class_et in course_et.findall('class'):
                if my_class.name == class_et.get('name'):
                    correct_class = True
                    for lecture_et in class_et.findall('lecture'):
                        date = lecture_et.get('date')
                        lecture = Lecture(date_name = date, my_class = my_class)
                        for video_et in lecture_et.findall('video'):
                            path = video_et.get('path')
                            video = Video({'total_path' : path}, lecture = lecture) 
                            video_list.append(video)
                        for audio_et in lecture_et.findall('audio'):
                            path = audio_et.get('path')
                            audio = Audio({'total_path' : path}, lecture = lecture) 
                            audio_list.append(audio)
    if not correct_course:
        return -1
    if not correct_class:
        return -2
    return video_list, audio_list

def prettify_all(fname):
    tree = ET.parse(fname)
    root = tree.getroot()
    prettified = _prettify(root)
    return prettified

def _prettify(elem):
    """Return a pretty-printed XML string for the Element.
    """
    rough_string = ET.tostring(elem, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent="  ")