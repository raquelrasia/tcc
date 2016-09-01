from django.conf.urls import patterns, include, url
from django.contrib import admin
from django.views.generic import TemplateView
from bookmarks.views import *
import os.path


site_media = os.path.join(os.path.dirname(__file__), 'site_media')
videos = os.path.join(os.path.dirname(__file__), 'videos')


urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'djangobookmarks.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),
    url(r'^admin/', include(admin.site.urls)),
    url(r'^$', main_page),
    url(r'^user/(\w+)/$', user_page),
    url(r'^login/$', 'django.contrib.auth.views.login'),
    url(r'^logout/$', logout_page),
    url(r'^site_media/(?P<path>.*)$', 'django.views.static.serve', 
    		{ 'document_root': site_media }),
    url(r'^videos/(?P<path>.*)$', 'django.views.static.serve', 
    		{ 'document_root': videos }),
    url(r'^register/$', register_page),
    url(r'^register/success/$', 
    		TemplateView.as_view( template_name = 'registration/registration_success.html'),
    		name = '/register/success'),
    #url(r'^save/$', bookmark_save_page),
    url(r'^tag/([^\s]+)$', tag_page),
    url(r'^tag/$', tag_cloud_page),
    url(r'^search/$', search_page),
    url(r'^class/$', course_list_page),
    url(r'^class/(\w+)/$', course_page),
    url(r'^course_save/$', course_save_page),
    url(r'^class/(\w+)/(\w+)/video_upload/$', video_upload_page),
    url(r'^api/video_upload/$', api_video_upload),
    url(r'^api/teacher_xml_download/$', api_return_teacher_info),
    url(r'^api/video_xml_download/$', api_return_videos_file), 
    url(r'^class/(\w+)/class_save/$', class_save_page),
    url(r'^class/(\w+)/(\w+)/$', class_page),   
    url(r'^index/$', index),
)
