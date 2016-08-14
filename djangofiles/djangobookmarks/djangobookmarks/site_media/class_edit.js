function class_edit(){
	var item = $(this).parent();
	var code =  item.find(".title").attr("href").split("/class/").pop();
	item.load("/class_save/?ajax&code=" + escape(code), null, function(){
		$("#save-form").submit(class_save);	
	});
	return false;
};

function class_save(){
	var item = $(this).parent();
	var data = {
		code : item.find("#id_code").val(),
		name : item.find("#id_name").val(),
		'csrfmiddlewaretoken' : getCookie('csrftoken'),
	};
	$.post("/class_save/?ajax", data, function(result){
		if (result != "failure")
		{
			item.before($("li", result).get(0));
			item.remove();
			$("ul.cours .edit").click(class_edit);
		}
		else
		{
			alert("Failed to validate class before saving");
		}
	});
	return false;
};

$(document).ready(function(){
	$("ul.cours .edit").click(class_edit);
});

function getCookie(name) {
    var cookieValue = null;
    if (document.cookie && document.cookie != '') {
        var cookies = document.cookie.split(';');
        for (var i = 0; i < cookies.length; i++) {
            var cookie = jQuery.trim(cookies[i]);
            // Does this cookie string begin with the name we want?
            if (cookie.substring(0, name.length + 1) == (name + '=')) {
                cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                break;
            }
        }
    }
    console.log(cookieValue);
    return cookieValue;
};