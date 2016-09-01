from django.shortcuts import render
from django.http import HttpResponse, Http404, HttpResponseRedirect 
from django.template import RequestContext
from django.template.loader import get_template
from django.contrib.auth.models import User
from django.contrib.auth import authenticate, login, logout
from django.shortcuts import render_to_response, get_object_or_404
from bookmarks.forms import *
from bookmarks.models import *
from django.contrib.auth.decorators import login_required
from django.db.models import Q
from django.core.exceptions import ObjectDoesNotExist
from django.http import JsonResponse
from datetime import datetime
from django.views.decorators.csrf import csrf_exempt
from xml_funcs import *
import os
from djangobookmarks.settings import MEDIA_ROOT
from django.utils.encoding import smart_str


# Create your views here.
def main_page(request):
    return render_to_response(
        'main_page.html', RequestContext(request),
    )

def user_page(request, username):
    try:
        user = User.objects.get(username = username)
    except:
        raise Http404('Requested user not found.')

    classes = user.class_set.all()
    for my_class in classes: 
        course = my_class.course
        print(course.code)

    variables = RequestContext (request, {
        'username' : username,
        'classes': classes,
        'show_tags' : True,
    })      
    return render_to_response('user_page.html', variables)

def logout_page(request):
    logout(request)
    return HttpResponseRedirect('/')


def register_page(request):
    if request.method == 'POST':
        form = RegistrationForm(request.POST)
        if form.is_valid():
            user = User.objects.create_user(
                username = form.cleaned_data["username"],
                password = form.cleaned_data["password1"],
                email = form.cleaned_data["email"],
            )
            return HttpResponseRedirect('/register/success')
    else:
        form = RegistrationForm()

    variables = RequestContext(request, 
        {
            'form' : form,
        }
    )

    return render_to_response(
        'registration/register.html',
        variables
    )

# @login_required(login_url = '/login/')
# def bookmark_save_page(request):
#     if request.method == 'POST':
#         form = BookmarkSaveForm(request.POST)
#         if form.is_valid():
#             bookmark = _bookmark_save(request, form)
#             return HttpResponseRedirect(
#                 '/user/%s' % request.user.username
#             )   
#     elif request.GET.has_key('url'):
#         url = request.GET['url']
#         title = ''
#         tags = ''
#         try:
#             link = Link.objects.get(url = url)
#             bookmark = Bookmark.objects.get(
#                 link = link,
#                 user = request.user,
#             )
#             title =  bookmark.title
#             tags = ' '.join(
#                 tag.name for tag in bookmark.tag_set.all()
#             )
#         except ObjectDoesNotExist:
#             pass

#         form = BookmarkSaveForm(
#             {
#                 'url' : url,
#                 'title' : title,
#                 'tags' : tags,
#             }
#         )
#     else:
#         form = BookmarkSaveForm()

#     variables = RequestContext(request, 
#         {
#             'form' : form,
#         }
#     )
#     return render_to_response('bookmark_save.html', variables)

def tag_page(request, tag_name):
    tag = get_object_or_404(Tag, name = tag_name)
    bookmarks = tag.bookmarks.order_by('-id')
    variables= RequestContext(request,{
        'bookmarks' : bookmarks,
        'tag_name' : tag_name,
        'show_tags' : True,
        'show_user' : True,
        }
    )
    return render_to_response('tag_page.html', variables)

def tag_cloud_page(request):
    MAX_WEIGHT = 5
    tags = Tag.objects.order_by('name')
    my_tag = tags[0]
    min_count = max_count = tags[0].bookmarks.count()
    for tag in tags:
        tag.count = tag.bookmarks.count()
        if  tag.count < min_count:
            min_count = tag.count()
        if max_count < tag.count:
            max_count = tag.count

    my_range = float(max_count - min_count)
    if my_range == 0.0:
        my_range = 1.0  
    for tag in tags:        
        tag.weight = int(MAX_WEIGHT * (tag.count - min_count)/my_range) 

    variables = RequestContext(request, {       
        'tags' : tags       
        }
    )

    return render_to_response('tag_cloud_page.html', variables)

def search_page(request):
    form = SearchForm()
    course = []
    show_results = False
    if request.GET.has_key('query'):
        show_results = True
        query = request.GET['query'].strip()
        if query:
            form = SearchForm({'query' : query})
            course = \
                Course.objects.filter(Q(name__icontains = query) | Q(code__icontains = query))[:10]

    variables = RequestContext(request, {
            'form' : form,
            'course' : course,
            'show_results' : show_results,
            'show_tags': True,
            'show_user': True,
            }
    )
    if request.GET.has_key('ajax'):
        return render_to_response('course_list.html', variables)
    else:
        return render_to_response('search.html', variables)

@login_required(login_url = '/login/')
def course_save_page(request):
    ajax = request.GET.has_key('ajax')
    if request.method == 'POST':
        form = CourseSaveForm(request.POST)
        if form.is_valid():
            course = _course_save(request, form)
            if ajax:
                variables = RequestContext (request, {
                    'course' : [course],
                    'show_edit' : True,
                })      
                return render_to_response('course_list_page.html', variables)
            else:   
                return HttpResponseRedirect(
                    '/class/%s' % course.code
                )   
        else: 
            if ajax:
                return HttpResponse('failure')

    elif request.GET.has_key('code'):
        code = request.GET['code']
        name = ''
        try:
            course = Course.objects.get(code = code)
            name =  course.name
        except ObjectDoesNotExist:
            pass
        form = CourseSaveForm(
            {
                'name' : name,
                'code' : code,
            }
        )
    else:
        form = CourseSaveForm()

    variables = RequestContext(request, 
        {
            'form' : form,
        }
    )
    if ajax:
        return render_to_response('course_save_form.html', variables)
    else:
        return render_to_response('course_save.html', variables)

def video_upload_page(request, course_code, class_name):
    if request.method == 'POST':
        form = VideoUploadForm(request.POST, request.FILES)
        if form.is_valid():
            file = request.FILES['file']
            video = Video(file = file)
            video.date =  form.cleaned_data['date'] 
            try:
                course = Course.objects.get(code = course_code)
                classes = course.class_set.all() 
                for c in classes:
                    if c.name  == class_name:
                        my_class = c
                        break;
            except:
                return HttpResponseRedirect('/')
            tag_names = form.cleaned_data['tags'].split()
            for tag_name in tag_names:
                tag, dummy = Tag.objects.get_or_create(
                    name = tag_name
                )
            
            #video.tag_set.add(tag)   
            my_class.video_set.add(video)
            video.save()
            #adiciona video ao arquivo xml da class
            _add_video_to_xml(course, my_class, video)

            course.save()
            return HttpResponseRedirect(
                '/class/%s' % course.code
            )
    else:
        form = VideoUploadForm()

    variables = RequestContext(request, 
        {
            'form' : form,
        }
    )
    return render_to_response('video_upload.html', variables)


def course_page(request, course_code):
    show_button = False
    try:
        show_button = True
        course = request.user.course_set.get(code = course_code)
    except:
        try:
            show_button = True
            course = Course.objects.get(code = course_code)
        except:    
            raise Http404('Disciplina nao encontrada.')
    
    classes = course.class_set.all()
    #videos = course.video_set.all()
    variables = RequestContext (request, {
        'course' : course,
        'classes': classes,
        'show_tags': False,
        'show_button' : show_button
    })      
    return render_to_response('course_page.html', variables)

def course_list_page(request):
    course = Course.objects.all() 
    variables = RequestContext (request, {
        'course' : course,
        'show_edit' : True,
    })      
    return render_to_response('course_list_page.html', variables)

# def _bookmark_save(request, form):
#     #Create or get a link
#     link, dummy = Link.objects.get_or_create(
#         url = form.cleaned_data['url']
#     )
#     #Create or get a bookmark
#     bookmark, created = Bookmark.objects.get_or_create(
#         user = request.user,
#         link = link,
#     )   
#     bookmark.title = form.cleaned_data['title']
#     if not created:
#         bookmark.tag_set.clear()

#     tag_names = form.cleaned_data['tags'].split()
#     for tag_name in tag_names:
#         tag, dummy = Tag.objects.get_or_create(
#             name = tag_name
#         )
#         bookmark.tag_set.add(tag)
#         #tag.bookmarks.add(bookmark)
#     bookmark.save()
#     return bookmark


def _course_save(request, form):
    
    print(form.cleaned_data['code'])
    course, created = Course.objects.get_or_create(
        code = form.cleaned_data['code'],
        #user_creator = request.user,
    )

    #print(course)
    course.name = form.cleaned_data['name']
    course.save()
    #print(course.name)
    return course

def class_save_page(request, course_code):
    #course = request.user.course_set.get(code = course_code)
    course =  Course.objects.get(code = course_code)
    if course:
        if request.method == 'POST':
            form = ClassSaveForm(request.POST, request.FILES)
            if form.is_valid():
                my_class = Class(name = form.cleaned_data['name'])
                my_class.year = form.cleaned_data['year']
                my_class.semester = form.cleaned_data['semester']
                my_class.user_teacher = request.user
                course.class_set.add(my_class)
                #caso seja professorclass_set
                create_xml_teacher_file(request.user)
                add_course_class(course, my_class, request.user)
                return HttpResponseRedirect(
                        '/class/{}/{}'.format(course_code, my_class.name) #% course_code, class.name
                    )
        else:
            form = ClassSaveForm()
        variables = RequestContext(request, 
            {
                'course' : course,
                'form' : form,
            }
        )
        return render_to_response('class_save.html', variables)          
    else:
        return HttpResponseRedirect('/')

def class_page(request, course_code, class_code):
    try:
        course = Course.objects.get(code = course_code)
        my_class = course.class_set.get(name = class_code)
    except:
        raise Http404('Disciplina/Class nao encontrada.')

    videos = my_class.video_set.all()
    variables = RequestContext (request, {
        'course': course,
        'class': my_class,
        'videos' : videos,
    })      
    return render_to_response('class_page.html', variables)



def index(request):
    response = JsonResponse({'foo':'bar'})
    # Get the number of visits to the site.
    # We use the COOKIES.get() function to obtain the visits cookie.
    # If the cookie exists, the value returned is casted to an integer.
    # If the cookie doesn't exist, we default to zero and cast that.
    visits = request.session.get('visits')

    reset_last_visit_time = False
 
    # Does the cookie last_visit exist?
    if 'last_visit' in request.COOKIES:
        # Yes it does! Get the cookie's value.
        last_visit = request.COOKIES['last_visit']
        # Cast the value to a Python date/time object.
        last_visit_time = datetime.strptime(last_visit[:-7], "%Y-%m-%d %H:%M:%S")

        # If it's been more than a day since the last visit...
        #if (datetime.now() - last_visit_time).days > 0:
        visits = visits + 1
            # ...and flag that the cookie last visit needs to be updated
        reset_last_visit_time = True
    else:
        print('nenhuma visita anterior')
        # Cookie last_visit doesn't exist, so flag that it should be set.
        reset_last_visit_time = True

    if reset_last_visit_time:
        response.set_cookie('last_visit', datetime.now())
        response.set_cookie('visits', visits)

    # Return response back to the user, updating any cookies that need changed.
    return response

"""
################################################################################################
#                                           API FUNCTIONS                                      #
################################################################################################
"""
#@login_required(login_url = '/login/')
@csrf_exempt
def api_video_upload(request):
    if request.method == 'POST':
        print('hehehe')
        form = VideoUploadForm_API(request.POST, request.FILES)
        if form.is_valid():
            video = Video(file = request.FILES['file'])
            video.date = form.cleaned_data['date']
            try:
                course = Course.objects.get(
                        code = form.cleaned_data['course_code']
                    )
            except:             
                response = JsonResponse({'state': 'class does not exist'})
                return response
            
            try:
                my_class = course.class_set.get(
                        name = form.cleaned_data['class_name'],
                        semester = form.cleaned_data['class_semester'],
                        year = form.cleaned_data['class_year'],
                    )
            except:
                response = JsonResponse({'state': 'class does not exist'})
                return response

            if form.cleaned_data.has_key('tags'):
                tag_names = form.cleaned_data['tags'].split()
                for tag_name in tag_names:
                    tag, dummy = Tag.objects.get_or_create(
                        name = tag_name
                    )
            my_class.video_set.add(video)
            video.save()
            _add_video_to_xml(course, my_class, video)
            response = JsonResponse({'state': 'video uploaded'})
        else:
            response = JsonResponse({'state': 'invalid form'})
        return response
    else:
        response = JsonResponse({'state': 'no gets allowed'})
        return HttpResponseRedirect('/')

#@login_required(login_url = '/login/')
#@csrf_exempt
def api_return_teacher_info(request):
    if request.user.is_authenticated():
        print('to logadoooohhh')
    response = HttpResponse()
    if request.GET.has_key('teacher_username'):
        user_name = request.GET['teacher_username']
    else:
        response.text = 'invalid username'
        return response

    file_name = MEDIA_ROOT + '/teachers/' + user_name + '.xml'
    xml_file = open(file_name, 'r')
    path_to_file = os.path.dirname(file_name)
    response = HttpResponse(xml_file.read() ,content_type='application/xml') # mimetype is replaced by content_type for django 1.7
    response['Content-Disposition'] = 'attachment; filename=%s' % file_name
    response['Content-Length'] = os.path.getsize(file_name)
    xml_file.close()
    return response

def api_return_videos_file(request):
    try:
        course_code = request.GET['course_code']
        class_name = request.GET['class_name']
        class_year = request.GET['class_year']
        class_semester = request.GET['class_semester']
        username = request.GET['username']
    except:
        response = HttpResponse('invalid params')
        return response

    try:
        course = Course.objects.get(code = course_code)
        my_class = course.class_set.get(name = class_name,
                                        year = class_year,
                                        semester = class_semester)
        user = User.objects.get(username = username)
    except:
         response = HttpResponse('invalid params')

    if my_class not in user.class_set.all():
        return response

    file_name = MEDIA_ROOT + '/videos/'  + get_course_class_path(course, my_class)
    names_split = file_name.split('/')
    name = names_split[-2] + names_split[-1]
    file_name =  file_name + name + '.xml' 
    xml_file = open(file_name, 'r')
    path_to_file = os.path.dirname(file_name)
    response = HttpResponse(xml_file.read() ,content_type='application/xml') # mimetype is replaced by content_type for django 1.7
    response['Content-Disposition'] = 'attachment; filename=%s' % file_name
    response['Content-Length'] = os.path.getsize(file_name)
    xml_file.close()
    return response
    
def _add_video_to_xml(course, my_class, video):
    fname = create_xml_video_file(course, my_class)
    add_video(video, fname)