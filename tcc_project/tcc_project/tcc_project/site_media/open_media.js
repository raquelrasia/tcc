function open_video(){
	var item = $(this);
	var video_open_elem = $(this).parent().find("#video_place");
	var close_video = $(this).parent().find("#close_video");
	video_open_elem.show();
	var path =  item.attr("href").split("/video_view/").pop();
	video_open_elem.load("/video_view/?ajax&path=" + escape(path), null, function(){
	});
	item.hide();
	close_video.show();
	return false;
};

function close_video(){
	var item = $(this);
	var open_video = $(this).parent().find(".video_media");
	var video_open_elem = $(this).parent().find("#video_place");
	video_open_elem.children().filter("video").each(function(){
	    this.pause(); // can't hurt
	    $(this).empty(); // this is probably what actually does the trick
	    $(this).remove(); // this is probably what actually does the trick
	    delete this; // @sparkey reports that this did the trick (even though it makes no sense!)
	});
	item.hide();
	open_video.show();
	return false;
};

function open_audio(){
	var item = $(this);
	var audio_open_elem = $(this).parent().find("#audio_place");
	var close_audio = $(this).parent().find("#close_audio");
	audio_open_elem.show();
	var path =  item.attr("href").split("/audio_view/").pop();
	audio_open_elem.load("/audio_view/?ajax&path=" + escape(path), null, function(){
	});
	item.hide();
	close_audio.show();
	console.log("ta");
	return false;
};

function close_audio(){
	var item = $(this);
	var open_audio = $(this).parent().find(".audio_media");
	var audio_open_elem = $(this).parent().find("#audio_place");
	audio_open_elem.children().filter("audio").each(function(){
	    this.pause(); // can't hurt
	    $(this).empty(); // this is probably what actually does the trick
	    $(this).remove(); // this is probably what actually does the trick
	    delete this; // @sparkey reports that this did the trick (even though it makes no sense!)
	});
	item.hide();
	console.log("ta2");
	open_audio.show();
	return false;
};
$(document).ready(function(){
	$("ul.videos .video_media").click(open_video);
	$("ul.videos #close_video").click(close_video);
	$("ul.videos #close_video").hide();
	
	$("ul.videos .audio_media").click(open_audio);
	$("ul.videos #close_audio").click(close_audio);
	$("ul.videos #close_audio").hide();
});