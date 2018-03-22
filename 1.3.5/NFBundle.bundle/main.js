/*============= Native 与 h5 交互 =============*/
/*
 * ios WebviewJavascriptBriage
 * android newslistener / imglistener
 */
var newsData = null;
var imgList = [];
var appInfo=null;
var baseUrl = null;

var producer = null;

var httpsOn = null;

var accuseData = {
reportType: null,
otherReason: "",
};


function consume_alert() {
    window.alert = function(message) {
        new PNotify({
                    title: message,
                    addclass: 'custom',
                    opacity: 0.80,
                    type: 'info',
                    animate_speed: "fast",
                    delay: 2000,
                    icon: false,
                    });
    };
}

consume_alert();

function resizeFontSize(size, response) {
    switch (size) {
        case 0:
            $(".content").removeClass("fs-md").removeClass("fs-lg").addClass("fs-sm");
            return;
        case 1:
            $(".content").removeClass("fs-sm").removeClass("fs-lg").addClass("fs-md");
            return;
        case 2:
            $(".content").removeClass("fs-sm").removeClass("fs-md").addClass("fs-lg");
            return;
        default:
            $(".content").removeClass("fs-sm").removeClass("fs-lg").addClass("fs-md");
            return;
    }
}

function setupWebViewJavascriptBridge(callback) {
    if (window.WebViewJavascriptBridge) {
        return callback(WebViewJavascriptBridge);
    }
    if (window.WVJBCallbacks) {
        return window.WVJBCallbacks.push(callback);
    }
    window.WVJBCallbacks = [callback];
    var WVJBIframe = document.createElement('iframe');
    WVJBIframe.style.display = 'none';
    WVJBIframe.src = 'https://__bridge_loaded__';
    document.documentElement.appendChild(WVJBIframe);
    setTimeout(function() { document.documentElement.removeChild(WVJBIframe) }, 0)
}

var intialData;
function initLoadData(data) {
    //debugger
    intialData=data;
    newsData = data.newsData;
    appInfo=data.appInfo;
    producer=data.producer;
    baseUrl = data.networkConfg;
    httpsOn = data.httpsOn;
    
    showShare=data.showShare;// 分享显示 控制
    formatTime();
    getRelatedData();
}

function getAppInfo(info){
    appInfo=info;
    getRelatedData();
}
function calcSignature(data) {
    var tempData = [];
    var secretkey = appInfo.appsecret;
    var codeStr = secretkey;
    
//    tempData = Object.keys(data).sort().map(key => {
//                                            if(key==="appsecret"){
//                                            return "";
//                                            }
//                                            return key + data[key];
//                                            });
//
    tempData = Object.keys(data).sort().map(function(key){
                                            if(key==="appsecret"){
                                            return "";
                                            }
                                            return key + data[key];
                                            });
    console.log(tempData);
    codeStr += tempData.join('');
    return md5(codeStr);
}
var relatedData;
var postdata;

function getRelatedData() {
    var url = baseUrl + "/api/v1/info/related";
    //todo 将appInfo中数据填充到data
    if(newsData==null||appInfo==null){
        return;
        
    }
    if (newsData.infoType !== "article") {
        return;
    }
    var data = Object.assign({}, appInfo, {
                             timestamp: +new Date,
                             infotype: newsData.infoType,
                             scene: 'r',
                             infoid: newsData.infoId,
                             });
    
    data.signature = calcSignature(data);
    
    delete data.appsecret;
    postdata = data;

    $.ajax(url, {
           data
           }).done(function(data) {
                   //responseData = data;
                   //{{if $data.data.infos}}
                   
                   relatedData = JSON.parse(data);
                   if(relatedData.code==0&&relatedData.data.infos&&relatedData.data.infos.length>0){
                   
                   if(httpsOn) {
                   for(j = 0,len=relatedData.data.infos.length; j < len; j++) {
                   var newsInfo = relatedData.data.infos[j];
                   if (newsInfo && newsInfo.thumbnails && newsInfo.thumbnails.length>0) {
                   for(i = 0,length=newsInfo.thumbnails.length; i < length; i++) {
                   var newsImage = newsInfo.thumbnails[i];
                   if(newsImage.url.indexOf("http://") > -1 ) {
                   var newstr=newsImage.url.replace("http://","https://");
                   newsImage.url = newstr;
                   }
                   }
                   }
                   }
                   }
                   
                   var html = template('realtedTMPL', relatedData.data.infos);
                   console.log(html);

                   $('.related').html(html);

                   }

                   newsDetail.render();
                   newsDetail.bindEvent();
                                      // console.log('relateData', data);

                   }).fail(function(err){
                           newsDetail.render();
                           newsDetail.bindEvent();
                           console.log(err);
                           })
}

function getImageFrame(data, responseCallback) {
    var index = data;
    var img = $('.content img').get(index);

    var x = 0; var y = 0;
    var element = img;
    while( element != null ) {
        x += element.offsetLeft + element.clientLeft;
        y += element.offsetTop + element.clientTop;
        element = element.offsetParent;

    }

    var width = img.clientWidth;
    var height = img.clientHeight;

    var pageOffset = window.pageYOffset;

    var responseData = {
        x : x,
        y : y,
        width : width,
        height : height,
        pageOffset : pageOffset
    };

    responseCallback(responseData);
}

function getImgList() {
    imgList = [];
    $(".content img").each(function() {
        imgList.push($(this).attr("src"));
    });
}

function webViewDidScroll() {
    $(window).trigger("lookup");
}

function loadLocalPath(index, src, isLocal) {
    var plat = newsDetail.getUA();
    if (plat == "ios") {
        src = index.src;
        isLocal = index.isLocal;
        index = index.index;
    }
    var img = $('.content img').eq(index);
    if (img) {
        img.attr("src", isLocal ? "file://" + src : src);
        img.load(function() {
            img.data('loaded', 1);
            img.data('loading', 0);
            img.width('auto');
            img.height('auto');
        });
    }
}

function loadImageError(index) {
    var plat = newsDetail.getUA();
    if (plat == "ios") {
        index = index.index;
    }
    var img = $('.content img').eq(index);
    if (img) {
        img.data('loaded', -1);
        img.next().show();
    }
}

function formatTime() {
    var dateStr = $("#news-pubtime").text().replace(/\-/g, '/');
    if (dateStr.length != 19) return;
    var date = new Date(dateStr);
    var time = Date.parse(dateStr);

    var now = new Date();
    var currentTime = +now;
    var today = +new Date(now.getFullYear(), now.getMonth(), now.getDate(), 0, 0, 0);
    var yesterday = +new Date(now.getFullYear(), now.getMonth(), now.getDate() - 1, 0, 0, 0);
    var b4yesterday = +new Date(now.getFullYear(), now.getMonth(), now.getDate() - 2, 0, 0, 0);

    var year = date.getFullYear();
    var month = date.getMonth() + 1;
    var hours = date.getHours();
    var minutes = date.getMinutes();
    if (hours < 10) hours = '0' + hours;
    if (minutes < 10) minutes = '0' + minutes;
    var hourAndMinute = [hours, minutes].join(':');

    var day = date.getDate();
    var span = now - time;
    span = Math.round(span / 1000);

    if (time < b4yesterday) {
        dateStr = year + "年" + month + "月" + day + "日" + hourAndMinute;
    } else if (time < yesterday && time >= b4yesterday) {
        dateStr = "前天" + hourAndMinute;
    } else if (time < today) {
        dateStr = "昨天" + hourAndMinute;
    } else if (span > 3600) {
        dateStr = Math.floor(span / 3600) + "小时前";
    } else if (span > 60 && span < 3600) {
        dateStr = Math.floor(span / 60) + "分钟前";
    } else if (span < 60) {
        dateStr = "刚刚"
    }
    $("#news-pubtime").text(dateStr).show();
}

function NewsDetail() {
    this.bridge = null;
    this.init();
}

NewsDetail.prototype = {
    init: function() {
        this.initIOS();
        this.initBorder();
    },
    bindEvent: function() {
        var self = this;
        var plat = self.getUA();
        $('.content img').on("click", { self: self }, function() {
            var loaded = $(this).data('loaded');
            if (loaded != '1' && loaded != '-1') return;

            var src = $(this).data("src");
            var index = $('.content img').index(this);

            var x = 0; var y = 0;
            var element = this;
            while( element != null ) {
                x += element.offsetLeft + element.clientLeft;
                y += element.offsetTop + element.clientTop;
                element = element.offsetParent;

            }

            var width = this.clientWidth;
            var height = this.clientHeight;

            var pageOffset = window.pageYOffset;

            if (loaded == '1') {
                if (plat == "ios") {
                    self.bridge.callHandler("receiveJavaScriptHandler", {
                        action: 1,
                        name: "tapImage",
                        data: {
                            src: src,
                            index: index,
                            x: x,
                            y: y,
                            width: width,
                            height: height,
                            pageOffset: pageOffset
                        }
                    }, function(response) {});
                } else if (plat == "android") {
                    imagelistner.openImage(index, src);
                } else {
                    return;
                }
            } else {
                $(this).data('loaded', '');
                $(this).data('loading', 1);
                $(this).next().hide();
                if (plat == "ios") {
                    self.bridge.callHandler("receiveJavaScriptHandler", {
                        action: 2,
                        name: "loadImage",
                        data: {
                            src: src,
                            index: index
                        }
                    }, function(response) {});
                } else if (plat == "android") {
                    imagelistner.loadImage(index, src);
                } else {
                    return;
                }
            }
        });
        $(".content a").on('click', function(e) { e.preventDefault(); });
        
        $(".ad").on('click', function(e) {
                               if (plat == "ios") {
                               self.bridge.callHandler("receiveJavaScriptHandler", {
                                                       action: 5,
                                                       name: "clickAd",
                                                       data: newsData
                                                       }, function(response) {});
                               }
                               });
        
        $("li.item").each(function(i,item) {
                     $(item).get(0).ontouchstart=function(e){
                          debugger
                        $(this).css("background","#f9f9f9");
                     };
                     $(item).get(0).ontouchend=function(){
                        $(this).css("background","white");
                     };
                     });
        
        $(".related .item").on('click', function(e) {
                               var index = $('.related .item').index(this);
                               var info = relatedData.data.infos[index];
                               if (plat == "ios") {
                               self.bridge.callHandler("receiveJavaScriptHandler", {
                                                       action: 4,
                                                       name: "clickNews",
                                                       data: info
                                                       }, function(response) {});
                               }
                              });
        
        
        
        
        
        
        $('.accuse .btn').on('click', function(e) {
                             var article = $('article'),
                             accusePage = $('.accusePage');
                             
                             
                             
                             if (plat == 'ios') {
                             self.bridge.callHandler("receiveJavaScriptHandler", {
                                                     action: 6,
                                                     name: "setTitle",
                                                     data: '报错',
                                                     }, function(response) { });
                             
                             } else if (plat == 'android') {
                             if (newslistner && newslistner.setTitle) newslistner.setTitle(newsData.source);
                             }
                             
                             article.hide();
                             accusePage.show();
                             })
        $('input[type=radio][name=optionItem]').change(function() {
                                                       var value = this.value;
                                                       
                                                       accuseData.reportType = parseInt(value, 10);
                                                       if (value == "9") {
                                                       $('.other').show()
                                                       $('.other')[0].scrollIntoView();

                                                       } else {
                                                       $('.other').hide()
                                                       }
                                                       });
        $(".other").on('change keyup paste', function() {
                       accuseData.otherReason = this.value;
                       });

        $(".submit").on('click', function(e) {
                        
                        var requesrUrl = baseUrl + "/api/v1/info/accuse";
                        if (accuseData.reportType === null) {
                        
                        alert('请选择投诉原因');
                        return;
                        }
                        var postdata = {
                        appkey: appInfo.appkey,
                        timestamp: +new Date,
                        infoid: newsData.infoId,
                        producer: producer,
                        reportType: accuseData.reportType,
                        otherReason: accuseData.otherReason,
                        };
                        postdata.signature = calcSignature(postdata);
                        $.ajax(requesrUrl, {
                               data: postdata,
                               }).done(function(data) {
                                       var res = JSON.parse(data);
                                       if (res.code == 0) {
                                       alert('投诉成功');
                                       
                                       if (plat == 'ios') {
                                       self.bridge.callHandler("receiveJavaScriptHandler", {
                                                               action: 7,
                                                               name: "setTitle",
                                                               data:newsData.source,
                                                               }, function(response) { });
                                       
                                       } else if (plat == 'android') {
                                       if (newslistner && newslistner.setTitle) newslistner.setTitle('报错');
                                       }

                                       $('article').show();
                                       $('.accusePage').hide();
                                       return
                                       }
                                       alert(res.message);
                        });
                           });

            $('#webchat').on('click tap', function() {
      self.handleEvent('share', { type: 0 });
    });

    $('#friends').on('click tap', function() {
      self.handleEvent('share', { type: 1 });
    });

    },
    getUA: function() {
        var UA = navigator.userAgent.toLowerCase(),
            plat = null;
        if (UA.match(/iphone|ipad|ipod/)) {
            plat = "ios";
        }
        if (UA.match(/android/)) {
            plat = "android";
        }
        return plat;
    },
    handleEvent: function(name, data, callback) {
        var self=this;
    var handleMap = {
      ios: {
        share: {
          action: 8,
          name: 'share'
        }
      },
      android: {
        share: {
          listener: 'newslistner',
          method: 'onShareClick',
        }
      }
    };
    var plat = this.getUA();

    var params = handleMap[plat][name];
    console.log(data);
    if (plat == "ios") {
      params.data = data || {};
      self.bridge.callHandler("receiveJavaScriptHandler",params, function(response) {});
    } else if (plat == 'android' && window[params.listener] && window[params.listener][params.method]) {
      window[params.listener][params.method].call(null, JSON.stringify(data));
    }

  },
    initIOS: function() {
        var self = this;
        setupWebViewJavascriptBridge(function(bridge) {
            self.bridge = bridge;
            self.bridge.registerHandler("resizeFontSize", resizeFontSize);
            self.bridge.registerHandler("initLoadData", initLoadData);
            self.bridge.registerHandler("loadLocalPath", loadLocalPath);
            self.bridge.registerHandler("loadImageError", loadImageError);
            self.bridge.registerHandler("webViewDidScroll", webViewDidScroll);
            self.bridge.registerHandler("imageFrame",getImageFrame);
            self.bridge.registerHandler("appInfo",getAppInfo);
        });
    },
    initBorder: function() {
        var ua = this.getUA();
        if (ua == "ios") {
            $("body").addClass("ios-device");
        } else {
            $("body").addClass("other-device");
        }
    },
    render: function() {
        var self = this;
        var plat = self.getUA();
        var content = $(".content").html();
        var defaultImgSrc = plat == "ios" ? "transparent.png" : "file:///android_asset/default/transparent.png";
        var imgs = newsData.imgs;
        var containerWidth = $(".content").width();
        if (!showShare) {
          $('.share').hide();
        }

        $(".content").html(content.replace(/\$\{\{(\d+)\}\}\$/g, function(match, p1) {
            if (imgs[p1]) {
                var imgSrc = imgs[p1].url;
                var width = imgs[p1].width || 100;
                var height = imgs[p1].height || 100;
                if (width > containerWidth) height = Math.round((height / width) * containerWidth);
                return '<span class="imgWrap"><img src="' + defaultImgSrc + '" data-src="' + imgSrc + '" style="width:' + width + 'px;height:' + height + 'px;"/><span class="tip">[点击加载]</span></span>';
            } else {
                return '';
            }
        }));
        if (plat == 'ios') {
            self.bridge.callHandler("receiveJavaScriptHandler", {
                action: 3,
                name: "initSuccess",
                data: newsData
            }, function(response) {});
        } else if (plat == 'android') {
            if (newslistner && newslistner.onPageFinished) newslistner.onPageFinished();
        }
        
        if (newsData&&newsData.ad&&newsData.ad.title) {
            var html = template('adTMPL', newsData.ad);
            console.log(html);
            
            $('.ad').html(html);
        }
        

        $(document).ready(function() {
            $("img").unveil($(window).height(), function() {
                var index = $('.content img').index(this);
                if (plat == 'ios') {
                    $('.content img').each(function(i) {
                        if (i <= index && $(this).data('loaded') != '1' && $(this).data('loading') != '1') {
                            $(this).data('loading', 1);
                            var src = $(this).data("src");
                            self.bridge.callHandler("receiveJavaScriptHandler", {
                                action: 2,
                                name: "loadImage",
                                data: {
                                    src: src,
                                    index: i
                                }
                            }, function(response) {});
                        }
                    });
                } else if (plat == 'android') {
                    var src = $(this).data("src");
                    imagelistner.loadImage(index, src);
                }
            });
        });
    }
}

var newsDetail = new NewsDetail();

