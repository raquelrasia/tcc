from django.db import models
from django.contrib.auth.models import User
from django.contrib.auth.signals import user_logged_in
import os
from djangobookmarks.settings import MEDIA_ROOT
from aux_funcs import get_course_class_path
#from aux_funcs import get_upload_file

# Create your models here.
class Link(models.Model):
    url = models.URLField(unique = True)
    
    def __str__(self):
        return self.url 


class Bookmark(models.Model):
    title = models.CharField(max_length = 200)
    user = models.ForeignKey(User)
    link = models.ForeignKey(Link)
    
    def __str__(self):
        return ("{}, {}".format(self.user.username, self.link.url))



class Course(models.Model):
    name = models.CharField(max_length = 200)
    code = models.CharField(max_length = 10)
    #class = models.ForeignKey(Class, null=True)
    #user_creator = models.ForeignKey(User)
    def __str__(self):
        return ("{}, {}".format(self.name, self.code))

class Class(models.Model):
    name = models.CharField(max_length = 4)
    year = models.PositiveIntegerField()
    #year = models.CharField(max_length = 4)
    semester = models.CharField(max_length = 1)
    course  = models.ForeignKey(Course, null = True)
    user_teacher = models.ForeignKey(User, null = True)

    def __str__(self):
        return ("{}, {}, {}".format(self.name, self.year, self.semester))

class Video(models.Model):
    def get_upload_file_path(self, filename):
        classes = self.my_class
        course = self.my_class.course
        path = 'videos/' + get_course_class_path(course, self.my_class) + filename  
        self.name = filename     
        return path

    file = models.FileField(upload_to = get_upload_file_path) #upload_to = get_upload_file_path))
    date = models.DateField()
    name = models.CharField(max_length = 100)
    my_class = models.ForeignKey(Class)

    def __str__(self):
        return ("{}, {}".format(self.file.url, self.date))



class Tag(models.Model):
    name = models.CharField(max_length = 64, unique = True)
    videos = models.ManyToManyField(Video)

    def __str__(self):
        return self.name

