import xml.etree.cElementTree as ET
import os
import datetime
from xml.dom import minidom
from models import Video

class XMLHandler():

    def create_xml_video_file(self,course, my_class): 
        path = MEDIA_ROOT +'videos/'+ get_course_class_path(course, my_class)

        names_split = path.split('/')
        name = names_split[-2] + names_split[-1]
        fname =  path + name + '.xml'

        if os.path.exists(fname):
            return fname
        else:
            if not os.path.exists(os.path.dirname(path)):
                os.makedirs(os.path.dirname(path))
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
        my_class = video.my_class
        course = my_class.course
        print(my_class, course)
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
                        size = ET.SubElement(video_et, 'size')
                        size.text = str(video.file.size)
                        date = ET.SubElement(video_et, 'date')
                        date.text = str(video.date.strftime('%d-%m-%Y'))

        if existing_class_course:
            f = open(fname, 'w')
            ET.tostring(root)
            #prettify(video_et)
            #f.write(ET.tostring(root))
            f.write(ET.tostring(root))#ET.tostring(root))
            f.close()




    def create_xml_teacher_file(self, user): 
        f_name = MEDIA_ROOT +'teachers/'+ user.username
        fname =  f_name + '.xml'
        if os.path.exists(fname) and os.path.size(fname) != 0:
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

    def add_course_class(self, course, my_class, user):
        f_name = MEDIA_ROOT +'teachers/'+ user.username
        fname =  f_name + '.xml'
        tree = ET.parse(fname)
        root = tree.getroot()
        existing_class = False
        for course_et in root.findall('course'):
            code = course_et.get('code')
            if code == course.code:
                #nova class em curso ja cadastrado
                existing_class = True
                _add_class(course_et, my_class)

        if not existing_class:
              course_et =  ET.SubElement(root, 'course')
              course_et.set('code', course.code)
              course_et.set('name', course.name)
              _add_class(course_et, my_class)

        f = open(fname, 'w')
        ET.tostring(root)
        #prettify(video_et)
        #f.write(ET.tostring(root))
        f.write(ET.tostring(root))#ET.tostring(root))
        f.close()

    def _add_class(self, course_node, my_class):
        class_et =  ET.SubElement(course_node, 'class')
        class_et.set('name', my_class.name)
        class_et.set('year', str(my_class.year))
        class_et.set('semester', str(my_class.semester))
        
    def prettify(elem):
        """Return a pretty-printed XML string for the Element.
        """
        rough_string = ET.tostring(elem, 'utf-8')
        reparsed = minidom.parseString(rough_string)
        return reparsed.toprettyxml(indent="  ")

    def compare_files(self, f_remote, f_local, course, my_class):
        print(os.path.abspath(f_remote))
        if not os.path.isfile(f_remote):
            return -1
        if not os.path.isfile(f_local):
            return -2

        missing_files = list()
        remote_video_list = list()
        local_video_list = list()
        remote_video_list = self._return_video_list(f_remote, course, my_class)
        local_video_list = self._return_video_list(f_local, course, my_class)

        for video in local_video_list:
            print('remote ', video.path)
            if video not in remote_video_list:
                #file needs to be uploaded
                missing_files.append(video)

        return missing_files 

    def _return_video_list(self,  fname, course, my_class):
        tree = ET.parse(fname)
        root = tree.getroot()
        correct_class = False
        correct_course = False
        video_list = list()

        for course_et in root.findall('course'):
            code = course_et.get('code')
            if code == course.code:
                correct_course = True
                for class_et in course_et.findall('class'):
                    if my_class.name == class_et.get('name'):
                        correct_class = True
                        #adiciona videos a lista
                        for video_et in class_et.findall('video'):
                            date = video_et.find('date').text
                            path = video_et.get('path')
                            video = Video(course, my_class, date, {'total_path' : path}) 
                            video_list.append(video)

        if not correct_course:
            return -1
        if not correct_class:
            return -2

        return video_list