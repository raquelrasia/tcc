from django.db import models
from django.contrib.auth.models import User
from django.contrib.auth.signals import user_logged_in

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

class Cours(models.Model):
	name = models.CharField(max_length = 200)
	code = models.CharField(max_length = 10)
	user_teacher = models.ForeignKey(User)
	def __str__(self):
		return ("{}, {}".format(self.name, self.code))

class Video(models.Model):
	file = models.FileField(upload_to='%Y/%m/%d')
	cours = models.ForeignKey(Cours)

	def __str__(self):
		return ("{}, {}".format(self.file.url, self.cours))


class Tag(models.Model):
	name = models.CharField(max_length = 64, unique = True)
	videos = models.ManyToManyField(Video)

	def __str__(self):
		return self.name
		
