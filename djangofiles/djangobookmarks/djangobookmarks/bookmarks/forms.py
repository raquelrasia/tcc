from django import forms
from django.contrib.auth.models import User
from django.core.exceptions import ObjectDoesNotExist
import re

class RegistrationForm(forms.Form):
	username = forms.CharField(label = 'Nome de usuario', max_length = 30)
	email = forms.EmailField(label = 'Email')
	password1 = forms.CharField(
		label = 'Senha',
		widget = forms.PasswordInput()
	)

	password2 = forms.CharField(
		label = 'Senha (Repita)',
		widget = forms.PasswordInput()
	)
	
	def clean_password2(self) :
		if 'password1' in self.cleaned_data:
			password1 = self.cleaned_data['password1']
			password2 = self.cleaned_data['password2']
			if (password1 == password2):
				return password2
		raise forms.ValidationError('Passwords inseridos sao diferentes')

	def clean_username(self) :
		username = self.cleaned_data['username']
		if not re.search(r'^\w+$', username):
			raise forms.ValidationError('Username deve conter apenas caracteres alfanumericos e undersocre')
		try:
			User.objects.get(username = username)
		except ObjectDoesNotExist:
			return username
		raise forms.ValidationError(' Username ja cadastrado')
		
class BookmarkSaveForm(forms.Form):
	url = forms.URLField(
		label = 'URL',
		widget = forms.TextInput(attrs = { 'size' : 64})
	)
	title = forms.CharField(
		label = 'Title',
		widget = forms.TextInput(attrs = { 'size' : 64})
	)
	tags = forms.CharField(
		label = 'Tags',
		required = False,
		widget = forms.TextInput(attrs = { 'size' : 64})
	)

kkkkkkkkkkkkkkkkkkkk teste
class SearchForm(forms.Form):
	query = forms.CharField(
		label = 'Digite uma Palavra-Chave para buscar',
		widget = forms.TextInput( attrs = {'size' : 32})
	)

class CoursSaveForm(forms.Form):
	name = forms.CharField(
		label = 'Nome',
		widget = forms.TextInput(attrs = { 'size' : 128})
	)
	code = forms.CharField(
		label = 'Codigo',
		widget = forms.TextInput(attrs = { 'size' : 10})
	)

class VideoUploadForm(forms.Form):
	file = forms.FileField(
		label  = 'Escolha um arquivo',
	)
	code = forms.CharField(
		label = 'Codigo da disciplina',
		widget = forms.TextInput(attrs = { 'size' : 10})
	)
	tags = forms.CharField(
		label = 'Tags',
		required = False,
		widget = forms.TextInput(attrs = { 'size' : 64})
	)
