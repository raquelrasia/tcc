

class XMLHandler():


import xml.etree.cElementTree as ET
from aux_funcs import get_course_class_path
from djangobookmarks.settings import MEDIA_ROOT
import os
import datetime
from xml.dom import minidom


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
    tree = ET.parse(fname)
    root = tree.getroot()
    video_et = ET.SubElement(root, 'video')
    video_et.set('name', video.name)
    video_et.set( 'path', video.file.name)
    size = ET.SubElement(video_et, 'size')
    size.text = str(video.file.size)
    date = ET.SubElement(video_et, 'date')
    date.text = str(video.date.strftime('%d-%m-%Y'))
    f = open(fname, 'w')
    ET.tostring(root)
    #prettify(video_et)
    #f.write(ET.tostring(root))
    f.write(ET.tostring(root))#ET.tostring(root))
    f.close()



def create_xml_teacher_file(user): 
    f_name = MEDIA_ROOT +'teachers/'+ user.username
    fname =  f_name + '.xml'
    if os.path.exists(fname):
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

def _add_class( course_node, my_class):
    class_et =  ET.SubElement(course_node, 'class')
    class_et.set('name', my_class.name)
    year = ET.SubElement(class_et, 'year')
    year.text = str(my_class.year)
    semester = ET.SubElement(class_et, 'semester')
    semester.text = str(my_class.semester)


def compare_files(f_remote, f_local, class_name, class_year, class_semester):
    missing_files = list()
    tree = ET.parse(f_remote)
    root_remote = tree.getroot()


    return missing_files

def prettify(elem):
    """Return a pretty-printed XML string for the Element.
    """
    rough_string = ET.tostring(elem, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent="  ")