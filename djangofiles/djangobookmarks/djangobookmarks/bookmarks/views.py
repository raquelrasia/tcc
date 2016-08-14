from django.shortcuts import render
from django.http import HttpResponse, Http404, HttpResponseRedirect 
from django.template import RequestContext
from django.template.loader import get_template
from django.contrib.auth.models import User
from django.contrib.auth import authenticate, login, logout
from django.shortcuts import render_to_response, get_object_or_404
from djangobookmarks.bookmarks.forms import *
from djangobookmarks.bookmarks.models import *
from django.contrib.auth.decorators import login_required
from django.db.models import Q
from django.core.exceptions import ObjectDoesNotExist
from django.http import JsonResponse
from datetime import datetime
from django.views.decorators.csrf import csrf_exempt

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

    cours = user.cours_set.all()
    variables = RequestContext (request, {
        'username' : username,
        'cours': cours,
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

@login_required(login_url = '/login/')
def bookmark_save_page(request):
    if request.method == 'POST':
        form = BookmarkSaveForm(request.POST)
        if form.is_valid():
            bookmark = _bookmark_save(request, form)
            return HttpResponseRedirect(
                '/user/%s' % request.user.username
            )   
    elif request.GET.has_key('url'):
        url = request.GET['url']
        title = ''
        tags = ''
        try:
            link = Link.objects.get(url = url)
            bookmark = Bookmark.objects.get(
                link = link,
                user = request.user,
            )
            title =  bookmark.title
            tags = ' '.join(
                tag.name for tag in bookmark.tag_set.all()
            )
        except ObjectDoesNotExist:
            pass

        form = BookmarkSaveForm(
            {
                'url' : url,
                'title' : title,
                'tags' : tags,
            }
        )
    else:
        form = BookmarkSaveForm()

    variables = RequestContext(request, 
        {
            'form' : form,
        }
    )
    return render_to_response('bookmark_save.html', variables)

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
    cours = []
    show_results = False
    if request.GET.has_key('query'):
        show_results = True
        query = request.GET['query'].strip()
        if query:
            form = SearchForm({'query' : query})
            cours = \
                Cours.objects.filter(Q(name__icontains = query) | Q(code__icontains = query))[:10]

    variables = RequestContext(request, {
            'form' : form,
            'cours' : cours,
            'show_results' : show_results,
            'show_tags': True,
            'show_user': True,
            }
    )
    if request.GET.has_key('ajax'):
        return render_to_response('class_list.html', variables)
    else:
        return render_to_response('search.html', variables)

#@login_required(login_url = '/login/')
@csrf_exempt
def api_video_upload(request):
    if request.method == 'POST':
        form = VideoUploadForm(request.POST, request.FILES)
        print('hmm')
        if form.is_valid():
            video = Video(file = request.FILES['file'])
            cours = Cours.objects.get(
                    code = form.cleaned_data['code']
                )
            if not cours:
                response = JsonResponse({'state': 'class does not exist'})
                return response
                
            tag_names = form.cleaned_data['tags'].split()
            for tag_name in tag_names:
                tag, dummy = Tag.objects.get_or_create(
                    name = tag_name
                )
                #video.tag_set.add(tag)

            cours.video_set.add(video)
            video.save()
            cours.save()
            response = JsonResponse({'state': 'video uploaded'})
    else:
         response = JsonResponse({'state': 'no gets allowed'})
    return response
    
@login_required(login_url = '/login/')
def class_save_page(request):
    ajax = request.GET.has_key('ajax')
    if request.method == 'POST':
        form = CoursSaveForm(request.POST)
        if form.is_valid():
            cours = _class_save(request, form)
            if ajax:
                variables = RequestContext (request, {
                    'cours' : [cours],
                    'show_edit' : True,
                })      
                return render_to_response('class_list_page.html', variables)
            else:   
                return HttpResponseRedirect(
                    '/class/%s' % cours.code
                )   
        else: 
            if ajax:
                return HttpResponse('failure')

    elif request.GET.has_key('code'):
        code = request.GET['code']
        name = ''
        try:
            cours = Cours.objects.get(code = code)
            name =  cours.name
        except ObjectDoesNotExist:
            pass
        form = CoursSaveForm(
            {
                'name' : name,
                'code' : code,
            }
        )
    else:
        form = CoursSaveForm()

    variables = RequestContext(request, 
        {
            'form' : form,
        }
    )
    if ajax:
        return render_to_response('class_save_form.html', variables)
    else:
        return render_to_response('class_save.html', variables)

def video_upload_page(request):
    if request.method == 'POST':
        form = VideoUploadForm(request.POST, request.FILES)
        if form.is_valid():
            video = Video(file = request.FILES['file'])
            cours, created = Cours.objects.get_or_create(
                    code = form.cleaned_data['code']
                )
            if created:
                cours.name = form.cleaned_data['code']
                
            tag_names = form.cleaned_data['tags'].split()
            for tag_name in tag_names:
                tag, dummy = Tag.objects.get_or_create(
                    name = tag_name
                )
                #video.tag_set.add(tag)
                
            cours.video_set.add(video)
            video.save()
            cours.save()
            return HttpResponseRedirect(
                '/class/%s' % cours.code
            )
    else:
        form = VideoUploadForm()

    variables = RequestContext(request, 
        {
            'form' : form,
        }
    )
    return render_to_response('video_upload.html', variables)


def class_page(request, class_code):
    try:
        cours = Cours.objects.get(code = class_code)
    except:
        raise Http404('Disciplina nao encontrada.')
    videos = cours.video_set.all()
    variables = RequestContext (request, {
        'cours' : cours,
        'videos': videos,
        'show_tags': False,
    })      
    return render_to_response('class_page.html', variables)

def class_list_page(request):
    cours = Cours.objects.all() 

    variables = RequestContext (request, {
        'cours' : cours,
        'show_edit' : True,
    })      
    return render_to_response('class_list_page.html', variables)

def _bookmark_save(request, form):
    #Create or get a link
    link, dummy = Link.objects.get_or_create(
        url = form.cleaned_data['url']
    )
    #Create or get a bookmark
    bookmark, created = Bookmark.objects.get_or_create(
        user = request.user,
        link = link,
    )   
    bookmark.title = form.cleaned_data['title']
    if not created:
        bookmark.tag_set.clear()

    tag_names = form.cleaned_data['tags'].split()
    for tag_name in tag_names:
        tag, dummy = Tag.objects.get_or_create(
            name = tag_name
        )
        bookmark.tag_set.add(tag)
        #tag.bookmarks.add(bookmark)
    bookmark.save()
    return bookmark


def _class_save(request, form):
    cours, created = Cours.objects.get_or_create(
        code = form.cleaned_data['code'],
        user_teacher = request.user
    )
    cours.name = form.cleaned_data['name']
    cours.save()
    print(cours.name)
    return cours


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