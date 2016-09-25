import xml.etree.cElementTree as ET
import os
import datetime
from xml.dom import minidom
from models import Video, Audio, MEDIA_ROOT



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

def add_audio(audio, fname):
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
                                exists = True
                                lecture_et.remove(video_et)
    if exists:
        f = open(fname, 'w')
        ET.tostring(root)
        #prettify(video_et)
        #f.write(ET.tostring(root))
        f.write(ET.tostring(root))#ET.tostring(root))
        f.close()



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
    for course_et in root.findall('course'):
        if course_et.get('code') == course.code:
            for class_et in course_et.findall('class'):
                if my_class.name == class_et.get('name'):
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


     
def remove_video(video, fname):
    lecture = video.lecture
    my_class = lecture.my_class
    course = my_class.course
    tree = ET.parse(fname)
    root = tree.getroot()
    existing_class_course = False
    for course_et in root.findall('course'):
        if course_et.get('code') == course.code:
            for class_et in course_et.findall('class'):
                if my_class.name == class_et.get('name'):
                    existing_class_course = True
                    video_et = ET.SubElement(class_et, 'video')
                    video_et.set('name', video.name)
                    video_et.set( 'path', video.file.name)
                    audio_et = ET.SubElement(class_et, 'audio')
                    audio_et.set( 'audio_path', video.audio_file.name)
                    size = ET.SubElement(video_et, 'size')
                    size.text = str(video.file.size)
    if existing_class_course:
        f = open(fname, 'w')
        ET.tostring(root)
        #prettify(video_et)
        #f.write(ET.tostring(root))
        f.write(ET.tostring(root))#ET.tostring(root))
        f.close()


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
    for video in local_video_list:
        if video not in remote_video_list:
            #file needs to be uploaded
            print ('missing video', video)
            missing_video_files.append(video)
    for audio in local_audio_list:
        if audio not in remote_audio_list:
            #file needs to be uploaded
            print ('missing audio', video)
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
                        for video_et in lecture_et.findall('video'):
                            path = video_et.get('path')
                            video = Video(course, my_class, date, {'total_path' : path},) 
                            video_list.append(video)
                        for audio_et in lecture_et.findall('audio'):
                            path = audio_et.get('path')
                            audio = Audio(course, my_class, date, {'total_path' : path}) 
                            audio_list.append(audio)
    if not correct_course:
        return -1
    if not correct_class:
        return -2
    return video_list, audio_list